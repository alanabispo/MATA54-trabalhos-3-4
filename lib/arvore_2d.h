/**
 * Definição da estrutura que armazena os índices
 * que permitem a busca aos metadados que contém
 * os ponteiros para os arquivos de texto onde
 * ocorre a busca de texto.
 */

#ifndef ARVORE_2D_H
#define ARVORE_2D_H

#include "registro.h"

// ------------- Constantes ------------- //

#include "constantes.h"
extern const char *PATH_ARQUIVO_INDICES;

// ------------- Estruturas ------------- //

/**
 * Define os tipos que um determinado nó de indice pode ter:
 *  - IMPAR = nome_autor
 *  - PAR = ano
 */
typedef enum {
    NO_IMPAR = 0,
    NO_PAR,
    NO_VAZIO
} TiposNosIndice;

/**
 * Tipo de apontamento que um índice pode utilizar
 */
typedef enum {
    APONTAMENTO_INDICE = 0,
    APONTAMENTO_PAGINA,
    APONTAMENTO_VAZIO
} TipoApontamento;

/**
 * CabecalhoNosIndices - Cabeçalho relacionado aos indices dos nós
 */
typedef struct {
    int n_indices;
} CabecalhoNosIndices;

/**
 * NoIndice - Nó que representa um indice da página
 */
typedef struct {
    // Armazena a propriedade principal em relação ao tipo do nó
    union {
        char nome_autor[TAMANHO_MAX_STRING + 1];
        int ano;
    };

    // Tipo do nó
    TiposNosIndice tipo;

    // Nível do nó
    int nivel;

    // Apontadores para os nós filhos do nó atual
    int direita;
    int esquerda;

    // Tipos dos apontadores
    TipoApontamento tipo_direita;
    TipoApontamento tipo_esquerda;
} NoIndice;

// --------------- Metodos -------------- //

/**
 * Cria um nó indice vazio
 *
 * @return Nó indice vazio.
 */
NoIndice newNoIndice();

/**
 * Cria cabecalho de nos indices vazio
 *
 * @return Cabecalho dos indices vazio.
 */
CabecalhoNosIndices newCabecalhoNosIndices();

/**
 * Cria arquivo de indices
 */
void criarArquivoIndices();

/**
 * Le cabecalho dos indices
 *
 * @return cabecalho dos indices
 */
CabecalhoNosIndices leCabecalhoIndices();

/**
 * Le indice do arquivo de indices
 *
 * @param pos Posição do arquivo a ser lida
 * @return Indice lido a partir do arquivo
 */
NoIndice leIndice(int pos);

/**
 * Grava cabecalho dos nos indices
 *
 * @param cabecalho Cabecalho a ser gravado
 */
void gravaCabecalhoIndices(CabecalhoNosIndices *cabecalho);

/**
 * Grava um indice no arquivo que representa a arvore
 *
 * @param pos Posição a ser gravada no arquivo
 * @param indice Dados do indice
 */
void gravaIndice(int pos, NoIndice *indice);

/**
 * Compara o registro com o indice atual com base no nível
 *
 * @param indice Indice atual
 * @param registro Registro a ser comparado
 * @return < 0 registro menor que indice, 0 se igual e > 0 se maior que indice
 */
int comparaNivel(NoIndice *indice, Registro *registro);

/**
 * Copia a propriedade nome autor para o indice
 *
 * @param indice Indice a ser copiado propriedade nome_autor
 * @param registro Registro de origem da propriedade nome_autor
 */
void copiaNomeAutorIndice(NoIndice *indice, Registro *registro);

/**
 * Inclui um novo indice a estrutura de indices, sem alterar as páginas
 *
 * @param idx Index do indice a ser adicionado
 * @param registro Registro a ser adicionado
 */
void incluirIndice(int idx, Registro *registro);

/**
 * Imprime todos os indices em ordem
 */
void imprimeIndices(int pos);

#endif //ARVORE_2D_H
