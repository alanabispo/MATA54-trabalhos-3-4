#pragma ide diagnostic ignored "misc-no-recursion"

#include "arvore_2d.h"
#include "pagina.h"
#include "registro.h"
#include "utils.h"

#include <stdio.h>
#include <string.h>

// Cria um nó indice vazio
NoIndice newNoIndice() {
    int i;
    NoIndice indice;

    // Inicializa os apontadores do índice
    indice.esquerda = APONTADOR_VAZIO;
    indice.direita = APONTADOR_VAZIO;

    // Inicializa outras propriedades
    indice.ano = ANO_VAZIO;
    indice.nivel = NIVEL_VAZIO;
    indice.tipo = NO_VAZIO;
    indice.tipo_direita = APONTAMENTO_VAZIO;
    indice.tipo_esquerda = APONTAMENTO_VAZIO;

    // Zera vetores
    for (i = 0; i < (TAMANHO_MAX_STRING + 1); i++) {
        indice.nome_autor[i] = '\0';
    }

    return indice;
}

// Cria cabecalho de nos indices vazio
CabecalhoNosIndices newCabecalhoNosIndices() {
    CabecalhoNosIndices cabecalho;

    cabecalho.n_indices = 0;

    return cabecalho;
}

// Cria arquivo de indices
void criarArquivoIndices() {
    FILE *arquivo = ABRIR_ARQUIVO_INDICES_ESCRITA();

    // Cria cabecalho e armazena no arquivo
    CabecalhoNosIndices cabecalho = newCabecalhoNosIndices();

    // Grava cabeçalho no arquivo
    fwrite(&cabecalho, TAMANHO_CABECALHO_INDICE(), 1, arquivo);
    fclose(arquivo);
}

// Deleta o arquivo de indices
void removeArquivoIndices() {
    remove(PATH_ARQUIVO_INDICES);
}

// Le cabecalho dos indices
CabecalhoNosIndices leCabecalhoIndices() {
    FILE *arquivo = ABRIR_ARQUIVO_INDICES_LEITURA();
    CabecalhoNosIndices cabecalho = newCabecalhoNosIndices();

    fread(&cabecalho, TAMANHO_CABECALHO_INDICE(), 1, arquivo);
    fclose(arquivo);

    return cabecalho;
}

// Le indice do arquivo de indices
NoIndice leIndice(int pos) {
    FILE *arquivo = ABRIR_ARQUIVO_INDICES_LEITURA();

    // Indice que será  retornando
    NoIndice indice = newNoIndice();

    // Caminha até o indice desejado
    fseek(arquivo, TAMANHO_CABECALHO_INDICE(), SEEK_CUR);
    fseek(arquivo, TAMANHO_INDICE() * pos, SEEK_CUR);

    // Lê um indice
    fread(&indice, TAMANHO_INDICE(), 1, arquivo);
    fclose(arquivo);

    return indice;
}

// Grava cabecalho dos nós indices
void gravaCabecalhoIndices(CabecalhoNosIndices *cabecalho) {
    FILE *arquivo = ABRIR_ARQUIVO_INDICES_LEITURA_ESCRITA();

    // Grava cabecalho dos indices
    fwrite(cabecalho, TAMANHO_CABECALHO_INDICE(), 1, arquivo);
    fclose(arquivo);
}

// Grava um indice no arquivo que representa a árvore
void gravaIndice(int pos, NoIndice *indice) {
    FILE *arquivo = ABRIR_ARQUIVO_INDICES_LEITURA_ESCRITA();

    // Caminha até o local desejado
    fseek(arquivo, TAMANHO_CABECALHO_INDICE(), SEEK_CUR);
    fseek(arquivo, TAMANHO_INDICE() * pos, SEEK_CUR);

    // Grava um indice
    fwrite(indice, TAMANHO_INDICE(), 1, arquivo);
    fclose(arquivo);
}

// Compara o registro com o indice atual com base no nível
int comparaNivel(NoIndice *indice, Registro *registro) {
    // Compara Nome autor no nível impar
    if (indice->nivel % 2 == 1) {
        return strncmp(registro->nome_autor, indice->nome_autor, TAMANHO_MAX_STRING + 1);
    }

    // Compara ano no nível par
    return registro->ano - indice->ano;
}

// Copia a propriedade nome autor para o indice
void copiaNomeAutorIndice(NoIndice *indice, Registro *registro) {
    strncpy(indice->nome_autor, registro->nome_autor, TAMANHO_MAX_STRING + 1);
}

/**
 * (privada) Função auxiliar para lidar com a recursividade da operação
 *
 * @param idx Index do item que vai ser adicionado
 * @param pos Posição da arvore sendo lida
 * @param nivel nivel do indice na arvore
 * @param registro Registro que está construindo o item
 */
void incluirIndiceRec(int idx, int pos, Registro *registro) {
    // Começamos a leitura a partir da raiz
    NoIndice indice = leIndice(pos);

    // Caminha para esquerda
    if (comparaNivel(&indice, registro) <= 0) {
        // Se a posição ainda não estiver sendo utilizada registra ela
        if (indice.tipo_esquerda == APONTAMENTO_VAZIO) {
            indice.tipo_esquerda = APONTAMENTO_INDICE;
            indice.esquerda = idx;

            // Grava indice pai
            gravaIndice(pos, &indice);

            // Grava novo indice vazio na nova posição
            NoIndice indice_filho = newNoIndice();
            indice_filho.nivel++;
            indice_filho.ano = registro->ano;
            indice_filho.tipo = NO_PAR;

            gravaIndice(idx, &indice_filho);

            return;
        }

        // Caminha pela esquerda
        incluirIndiceRec(idx, indice.esquerda, registro);
        return;
    }

    // Se a posição ainda não estiver sendo utilizada registra ela
    if (indice.tipo_direita == APONTAMENTO_VAZIO) {
        indice.tipo_direita = APONTAMENTO_INDICE;
        indice.direita = idx;

        // Grava indice pai
        gravaIndice(pos, &indice);

        // Grava novo indice vazio na nova posição
        NoIndice indice_filho = newNoIndice();
        indice_filho.nivel++;
        indice_filho.ano = registro->ano;
        indice_filho.tipo = NO_PAR;

        gravaIndice(idx, &indice_filho);

        return;
    }

    // Caminha Pela direita
    incluirIndiceRec(idx, indice.direita, registro);
}

// Inclui um novo indice a estrutura de indices, sem alterar as páginas
void incluirIndice(int idx, Registro *registro) {
    // Começamos a leitura a partir da raiz
    NoIndice indice = leIndice(0);

    // Incrementa o número de índices no cabeçalho
    CabecalhoNosIndices cabecalho = leCabecalhoIndices();
    cabecalho.n_indices++;
    gravaCabecalhoIndices(&cabecalho);

    // O primeiro index é a raiz
    if (idx == 0) {
        indice.tipo = NO_IMPAR;
        indice.nivel = NIVEL_RAIZ;

        // Copia o nome_autor para o indice
        copiaNomeAutorIndice(&indice, registro);

        // Grava o indice
        gravaIndice(0, &indice);

        return;
    }

    // Como não é a primeira posição, iremos seguir com as inserções recursivas
    if (comparaNivel(&indice, registro) <= 0) {
        // Se a posição ainda não estiver sendo utilizada registra ela
        if (indice.tipo_esquerda == APONTAMENTO_VAZIO) {
            indice.tipo_esquerda = APONTAMENTO_INDICE;
            indice.esquerda = idx;

            // Grava indice pai
            gravaIndice(0, &indice);

            // Grava novo indice vazio na nova posição
            NoIndice indice_filho = newNoIndice();
            indice_filho.nivel++;
            indice_filho.ano = registro->ano;
            indice_filho.tipo = NO_PAR;

            gravaIndice(1, &indice_filho);

            return;
        }

        // Caminha pela esquerda
        incluirIndiceRec(idx, indice.esquerda, registro);
        return;
    }

    // Se a posição ainda não estiver sendo utilizada registra ela
    if (indice.tipo_direita == APONTAMENTO_VAZIO) {
        indice.tipo_direita = APONTAMENTO_INDICE;
        indice.direita = idx;

        // Grava indice pai
        gravaIndice(0, &indice);

        // Grava novo indice vazio na nova posição
        NoIndice indice_filho = newNoIndice();
        indice_filho.nivel++;
        indice_filho.ano = registro->ano;
        indice_filho.tipo = NO_PAR;

        gravaIndice(1, &indice_filho);

        return;
    }

    // Caminha Pela direita
    incluirIndiceRec(idx, indice.direita, registro);
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

// Imprime todos os indices em ordem
void imprimeIndices(int pos) {
    NoIndice esquerda;
    NoIndice direita;
    NoIndice indice = leIndice(pos);

    // Não faz nada
    if (indice.tipo == NO_VAZIO) {
        return;
    }

    // Percorre a esquerda
    if (indice.tipo_esquerda == APONTAMENTO_INDICE) {
        imprimeIndices(indice.esquerda);
    }

    // Verifica qual o lado
    if (indice.tipo == NO_IMPAR) {
        printf("autor: %s ", indice.nome_autor);

        // Imprime o nó a esquerda
        if (indice.tipo_esquerda == APONTAMENTO_PAGINA) {
            printf("fesq: pagina ");
        }
        else {
            esquerda = leIndice(indice.esquerda);
            printf("fesq: %d ", esquerda.ano);
        }

        // Imprime o nó a direita
        if (indice.tipo_direita == APONTAMENTO_PAGINA) {
            printf("fdir: pagina ");
        }
        else {
            direita = leIndice(indice.direita);
            printf("fdir: %d\n", direita.ano);
        }
    }
    else {
        printf("ano: %d ", indice.ano);

        // Imprime o nó a esquerda
        if (indice.tipo_esquerda == APONTAMENTO_PAGINA) {
            printf("fesq: pagina ");
        }
        else {
            esquerda = leIndice(indice.esquerda);
            printf("fesq: %s ", esquerda.nome_autor);
        }

        // Imprime o nó a direita
        if (indice.tipo_direita == APONTAMENTO_PAGINA) {
            printf("fdir: pagina ");
        }
        else {
            direita = leIndice(indice.direita);
            printf("fdir: %s\n", direita.nome_autor);
        }
    }

    // Percorre a direita
    if (indice.tipo_direita == APONTAMENTO_INDICE) {
        imprimeIndices(indice.direita);
    }
}
