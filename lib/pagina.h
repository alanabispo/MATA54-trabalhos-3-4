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
 * Le pagina de registros do arquivo de páginas
 *
 * @param pos posição do arquivo a ser lida
 * @return Página de registros obtida do arquivo
 */
PaginaRegistros lePagina(int pos);

/**
 * Grava cabecalho paginas
 *
 * @param cabecalho Cabecalho a ser gravado
 */
void gravaCabecalhoPaginas(CabecalhoPaginasRegistros *cabecalho);

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


/**
 * Salva o registro na página correta ou retorna a posição que encontrou o mesmo caso exista
 *
 * @param registro Registro a ser buscado
 * @return Pagina e indice para salvar um documento
 */
LocalizacaoRegistroPagina salvaRegistroPagina(Registro *registro);

/**
 * Salva todas as páginas com os registros na lista de registros temporários
 *
 * @param n número de registros
 */
void moveRegistrosTmpPaginas(int n);

#endif //PAGINA_H
