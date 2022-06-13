#pragma ide diagnostic ignored "misc-no-recursion"

#include "pagina.h"
#include "arvore_2d.h"
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

// Grava cabecalho paginas
void gravaCabecalhoPaginas(CabecalhoPaginasRegistros *cabecalho) {
    FILE *arquivo = ABRIR_ARQUIVO_PAGINAS_LEITURA_ESCRITA();

    // Grava cabecalho dos registros
    fwrite(cabecalho, TAMANHO_CABECALHO_PAGINAS(), 1, arquivo);
    fclose(arquivo);
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

// Salva o registro na página correta ou retorna a posição que encontrou o mesmo caso exista
LocalizacaoRegistroPagina salvaRegistroPagina(Registro *registro) {
    int pos_pagina = POSICAO_INVALIDA;
    int pos_indice = 0;
    NoIndice indice;

    // Encontra a página adequada
    do {
        indice = leIndice(pos_indice);

        // Caminha pela esquerda
        if (comparaNivel(&indice, registro) <= 0) {
            if (indice.tipo_esquerda == APONTAMENTO_INDICE) {
                pos_indice = indice.esquerda;
            }
            else if (indice.tipo_esquerda == APONTAMENTO_PAGINA) {
                pos_pagina = indice.esquerda;
                break;
            }
            else {
                pos_indice = POSICAO_INVALIDA;
            }
        }
        else {
            if (indice.tipo_direita == APONTAMENTO_INDICE) {
                pos_indice = indice.direita;
            }
            else if (indice.tipo_direita == APONTAMENTO_PAGINA) {
                pos_pagina = indice.direita;
                break;
            }
            else {
                pos_indice = POSICAO_INVALIDA;
            }
        }
    } while (indice.tipo != NO_VAZIO && pos_indice != POSICAO_INVALIDA);

    // Problema encontrado
    if (indice.tipo == NO_VAZIO || pos_indice == POSICAO_INVALIDA || pos_pagina == POSICAO_INVALIDA) {
        return (LocalizacaoRegistroPagina){ .pagina = POSICAO_INVALIDA, .item = POSICAO_INVALIDA };
    }

    // Procura o registro na página ou nas páginas aninhadas
    //      SE Registro encontrado → retorna
    //      SENÃO Armazena
    //          SE Página estiver cheia, cria uma página antes
    //          Armazena o registro
    int i;
    int pos_pagina_atual = pos_pagina;
    PaginaRegistros pagina;
    do {
        pos_pagina = pos_pagina_atual;
        pagina = lePagina(pos_pagina_atual);

        // Procura o item na página atual
        for (i = 0; i < pagina.n_registros; i++) {
            if (equalsRegistro(&pagina.registros[i], registro)) {
                return (LocalizacaoRegistroPagina){.pagina = pos_pagina, .item = i };
            }
        }

        // Se a página estiver cheia e não tivermos encontrado caminha para próxima
        pos_pagina_atual = pagina.prox_pagina;
    } while (pagina.n_registros == NREGSPORPAGINA && pos_pagina_atual != APONTADOR_VAZIO);

    CabecalhoPaginasRegistros cabecalho = leCabecalhoPaginasRegistros();
    cabecalho.n_registros++;

    // Se a página não tem posições livres
    if (pagina.n_registros == NREGSPORPAGINA) {
        // Atualiza a página atual
        int pos_nova_pagina = cabecalho.total_paginas;
        pagina.prox_pagina = pos_nova_pagina;
        cabecalho.total_paginas++;
        gravaPagina(pos_pagina, &pagina);

        // Cria nova página
        pos_pagina = pos_nova_pagina;
        pagina = newPaginaRegistros();
    }

    // Insere o novo registro
    int pos_item = pagina.n_registros;
    pagina.n_registros++;
    cabecalho.n_registros++;
    pagina.registros[pos_item] = *registro;

    gravaCabecalhoPaginas(&cabecalho);
    gravaPagina(pos_pagina, &pagina);

    return (LocalizacaoRegistroPagina){.pagina = pos_pagina, .item = pos_item };
}

/**
 * (privado) Cria páginas de maneira recursiva em abordagem pre ordem
 *
 * @param pos Posição que está sendo lida
 * @param n_paginas Numero de páginas já registrado
 */
void criarPaginasRec(int pos, int *n_paginas) {
    NoIndice indice = leIndice(pos);

    // Acessa o nó a esquerda ou cria página
    if (indice.tipo_esquerda == APONTAMENTO_INDICE) {
        criarPaginasRec(indice.esquerda, n_paginas);
    } else if (indice.tipo_esquerda == APONTAMENTO_VAZIO) {
        indice.tipo_esquerda = APONTAMENTO_PAGINA;
        indice.esquerda = *n_paginas;

        // Atualiza o número de páginas
        CabecalhoPaginasRegistros cabecalho = leCabecalhoPaginasRegistros();
        cabecalho.total_paginas++;
        cabecalho.n_paginas_folhas++;
        gravaCabecalhoPaginas(&cabecalho);

        // Cria a página
        PaginaRegistros pagina = newPaginaRegistros();
        gravaPagina(*n_paginas, &pagina);

        // Incrementa o número de páginas
        *n_paginas = *n_paginas + 1;
    }

    // Acessa o nó a direita ou cria página
    if (indice.tipo_direita == APONTAMENTO_INDICE) {
        criarPaginasRec(indice.direita, n_paginas);
    } else if (indice.tipo_direita == APONTAMENTO_VAZIO) {
        indice.tipo_direita = APONTAMENTO_PAGINA;
        indice.direita = *n_paginas;

        // Atualiza o número de páginas
        CabecalhoPaginasRegistros cabecalho = leCabecalhoPaginasRegistros();
        cabecalho.total_paginas++;
        cabecalho.n_paginas_folhas++;
        gravaCabecalhoPaginas(&cabecalho);

        // Cria a página
        PaginaRegistros pagina = newPaginaRegistros();
        gravaPagina(*n_paginas, &pagina);

        // Incrementa o número de páginas
        *n_paginas = *n_paginas + 1;
    }
}

// Salva todas as páginas com os registros na lista de registros temporários
void moveRegistrosTmpPaginas(int n) {
    int i;
    int n_paginas = 0;
    Registro registro;

    // Percorre o indice em pré ordem e registra apontadores para as páginas
    criarPaginasRec(0, &n_paginas);

    // Salva os demais registros
    for (i = 0; i < n; i++) {
        registro = leRegistroTmp(i);
        salvaRegistroPagina(&registro);
    }
}