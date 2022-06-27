#pragma ide diagnostic ignored "misc-no-recursion"

#include "pagina.h"
#include "arvore_2d.h"
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


// Le pagina de registros do arquivo de páginas
PaginaRegistros lePagina(int pos) {
    FILE *arquivo = ABRIR_ARQUIVO_PAGINAS_LEITURA();

    // Página que será retornada
    PaginaRegistros pagina = newPaginaRegistros();

    // Caminha até a página desejada
    fseek(arquivo, TAMANHO_CABECALHO_PAGINAS(), SEEK_CUR);
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
