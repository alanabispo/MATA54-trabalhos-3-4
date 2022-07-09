#pragma ide diagnostic ignored "misc-no-recursion"

#include "pagina.h"
#include "registro.h"
#include "utils.h"

#include <stdio.h>

// Cria uma página de registros
PaginaRegistros newPaginaRegistros() {
    int i;
    PaginaRegistros pagina;

    // Zera propriedades
    pagina.n_registros = 0;
    pagina.prox_pagina = APONTADOR_VAZIO;

    // Zera os items da página
    for (i = 0; i < NREGSPORPAGINA; i++) {
        pagina.registros[i] = newRegistro();
    }

    return pagina;
}

// Cria cabecalho de página de registros vazio
CabecalhoPaginasRegistros newCabecalhoPaginasRegistros() {
    CabecalhoPaginasRegistros cabecalho;

    cabecalho.n_registros = 0;
    cabecalho.total_paginas = 0;
    cabecalho.n_paginas_folhas = 0;

    return cabecalho;
}

// Cria arquivo Páginas
void criarArquivoPaginas() {
    FILE *arquivo = ABRIR_ARQUIVO_PAGINAS_ESCRITA();

    CabecalhoPaginasRegistros cabecalho = newCabecalhoPaginasRegistros();
    fwrite(&cabecalho, TAMANHO_CABECALHO_PAGINAS(), 1, arquivo);

    fclose(arquivo);
}

// Remove arquivo paginas
void removeArquivoPaginas() {
    remove(PATH_ARQUIVO_PAGINAS);
}

// Le cabecalho das páginas
CabecalhoPaginasRegistros leCabecalhoPaginasRegistros() {
    FILE *arquivo = ABRIR_ARQUIVO_PAGINAS_LEITURA();
    CabecalhoPaginasRegistros cabecalho = newCabecalhoPaginasRegistros();

    fread(&cabecalho, TAMANHO_CABECALHO_PAGINAS(), 1, arquivo);
    fclose(arquivo);

    return cabecalho;
}

// Grava cabecalho paginas
void gravaCabecalhoPaginas(CabecalhoPaginasRegistros *cabecalho) {
    FILE *arquivo = ABRIR_ARQUIVO_PAGINAS_LEITURA_ESCRITA();

    // Grava cabecalho dos registros
    fwrite(cabecalho, TAMANHO_CABECALHO_PAGINAS(), 1, arquivo);
    fclose(arquivo);
}

// Compara duas páginas
bool equalsPagina(PaginaRegistros *pag_a, PaginaRegistros *pag_b) {
    if (pag_a->n_registros != pag_b->n_registros) {
        return false;
    }

    // Compara os registros da página atual
    int i;
    for (i = 0; i < pag_a->n_registros; i++) {
        if (!equalsRegistro(&pag_a->registros[i], &pag_b->registros[i])) {
            return false;
        }
    }

    // Verifica se existe próxima página. Se esta existe e olhamos o mesmo arq, são iguais
    // TODO encontrar maneira segura de verificar recursivamente
    //if (pag_a->prox_pagina != pag_b->prox_pagina) {
    //    return false;
    //}

    return true;
}

// Le pagina de registros do arquivo de páginas
PaginaRegistros lePagina(int pos) {
    FILE *arquivo = ABRIR_ARQUIVO_PAGINAS_LEITURA();

    // Lê o cabecalho
    CabecalhoPaginasRegistros cabecalho = newCabecalhoPaginasRegistros();
    fread(&cabecalho, TAMANHO_CABECALHO_PAGINAS(), 1, arquivo);

    // Página que será retornada
    PaginaRegistros pagina = newPaginaRegistros();

    // Pagina inexistente
    if (pos >= cabecalho.total_paginas) {
        pagina.n_registros = PAGINA_INEXISTENTE;
        return pagina;
    }

    // Caminha até a página desejada
    fseek(arquivo, TAMANHO_PAGINA() * pos, SEEK_CUR);

    // Lê uma página
    fread(&pagina, TAMANHO_PAGINA(), 1, arquivo);
    fclose(arquivo);

    return pagina;
}

// Grava uma página de registros na memória
void gravaPagina(int pos, PaginaRegistros *pagina) {
    FILE *arquivo = ABRIR_ARQUIVO_PAGINAS_LEITURA_ESCRITA();

    // Caminha até o local desejado
    fseek(arquivo, TAMANHO_CABECALHO_PAGINAS(), SEEK_CUR);
    fseek(arquivo, TAMANHO_PAGINA() * pos, SEEK_CUR);

    // Grava uma página
    fwrite(pagina, TAMANHO_PAGINA(), 1, arquivo);
    fclose(arquivo);
}

// Imprime a página com o índice especificado
void imprimePagina(int pos) {
    int i;
    PaginaRegistros pagina;

    printf("pagina: %d\n", pos);
    do {
        pagina = lePagina(pos);

        // Imprime os registros da página
        for (i = 0; i < pagina.n_registros; i++) {
            imprimeRegistro(&pagina.registros[i]);
        }

        pos = pagina.prox_pagina;
    } while(pagina.n_registros == NREGSPORPAGINA && pos != POSICAO_INVALIDA);
}
