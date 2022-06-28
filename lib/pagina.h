/**
 * Definição da estrutura de uma página de registros,
 * uma estrutura final para o armazenamento dos metadados
 * utilizados para encontrar arquivos de texto para busca.
 */
#ifndef PAGINA_H
#define PAGINA_H

#include "registro.h"

// ------------- Constantes ------------- //

#include "constantes.h"
extern const char *PATH_ARQUIVO_PAGINAS;

// ------------- Estruturas ------------- //

/**
 * CabeçalhoListaRegistros - Metadados para armazenamento dos registros numa tabela temporária
 */
typedef struct {
    int n_registros;
    int n_paginas_folhas;
    int total_paginas;
} CabecalhoPaginasRegistros;

/**
 * PaginaRegistros - Armazena os registros nos nós folhas da arvore k-d
 */
typedef struct {
    int n_registros;
    int prox_pagina;
    Registro registros[NREGSPORPAGINA];
} PaginaRegistros;

/**
 * LocalizacaoRegistroPagina - Dados de retorno a uma busca
 */
typedef struct {
    int pagina;
    int item;
} LocalizacaoRegistroPagina;

// --------------- Metodos -------------- //

/**
 * Cria cabecalho de página de registros vazio
 *
 * @return Cabecalho pagina de registros vazios
 */
CabecalhoPaginasRegistros newCabecalhoPaginasRegistros();

/**
 * Cria uma página de registros
 *
 * @return Página de registros vazia
 */
PaginaRegistros newPaginaRegistros();

/**
 * Cria arquivo Páginas
 */
void criarArquivoPaginas();

/**
 * Le cabecalho das páginas
 *
 * @return cabecalho das pagina
 */
CabecalhoPaginasRegistros leCabecalhoPaginasRegistros();

/**
 * Grava cabecalho paginas
 *
 * @param cabecalho Cabecalho a ser gravado
 */
void gravaCabecalhoPaginas(CabecalhoPaginasRegistros *cabecalho);

/**
 * Compara se duas páginas são idênticas
 *
 * @param pag_a Página A
 * @param pag_b Página B
 * @return Se as páginas informadas são idênticas
 */
bool equalsPagina(PaginaRegistros *pag_a, PaginaRegistros *pag_b);

/**
 * Le pagina de registros do arquivo de páginas
 *
 * @param pos posição do arquivo a ser lida
 * @return Página de registros obtida do arquivo
 */
PaginaRegistros lePagina(int pos);

/**
 * Grava uma página de registros na memória
 *
 * @param pos Posição da página a ser gravada
 * @param pagina Dados da página
 */
void gravaPagina(int pos, PaginaRegistros *pagina);

/**
 * Imprime a página com o índice especificado
 *
 * @param pos
 */
void imprimePagina(int pos);

#endif //PAGINA_H
