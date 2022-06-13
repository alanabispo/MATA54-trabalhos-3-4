#include "arvore_2d.h"
#include "registro.h"
#include "utils.h"

#include <stdio.h>
#include <string.h>

// Cria um nó indice vazio
NoIndice newNoIndice() {
    int i;
    NoIndice noIndice;

    // Inicializa os apontadores do índice
    noIndice.esquerda = APONTADOR_VAZIO;
    noIndice.direita = APONTADOR_VAZIO;

    // Inicializa outras propriedades
    noIndice.ano = ANO_VAZIO;
    noIndice.nivel = NIVEL_VAZIO;
    noIndice.tipo = NO_VAZIO;
    noIndice.tipo_direita = APONTAMENTO_VAZIO;
    noIndice.tipo_esquerda = APONTAMENTO_VAZIO;

    // Zera vetores
    for (i = 0; i < (TAMANHO_MAX_STRING + 1); i++) {
        noIndice.nome_autor[i] = '\0';
    }

    return noIndice;
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

    // Cria indice raiz e armazena no arquivo
    NoIndice no_raiz = newNoIndice();
    no_raiz.nivel = NIVEL_RAIZ;

    // Grava raiz no arquivo
    fwrite(&no_raiz, TAMANHO_INDICE(), 1, arquivo);
    fclose(arquivo);
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