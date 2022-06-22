/**
 * Macros e outros utilitários comuns ao projeto
 */

#ifndef UTILS_H
#define UTILS_H

// ------------- Constantes ------------- //

#include "constantes.h"
extern const char *PATH_ARQUIVO_PAGINAS;
extern const char *PATH_ARQUIVO_INDICES;
extern const char *PATH_ARQUIVO_TEMP;

// --------- Macros Utilitárias --------- //

/** Máximo entre duas variáveis numéricas de tipo compatíveis */
#define max(A, B) ((A) > (B) ? (A) : (B))

/** Mínimo entre duas variáveis numéricas de tipo compatíveis */
#define min(A, B) ((A) < (B) ? (A) : (B))

/** Operações no arquivo de indices */
#define ABRIR_ARQUIVO_INDICES_LEITURA()               fopen(PATH_ARQUIVO_INDICES, "rb")
#define ABRIR_ARQUIVO_INDICES_ESCRITA()               fopen(PATH_ARQUIVO_INDICES, "wb")
#define ABRIR_ARQUIVO_INDICES_LEITURA_ESCRITA()       fopen(PATH_ARQUIVO_INDICES, "rb+")

/** Operações no arquivo de páginas */
#define ABRIR_ARQUIVO_PAGINAS_LEITURA()               fopen(PATH_ARQUIVO_PAGINAS, "rb")
#define ABRIR_ARQUIVO_PAGINAS_ESCRITA()               fopen(PATH_ARQUIVO_PAGINAS, "wb")
#define ABRIR_ARQUIVO_PAGINAS_LEITURA_ESCRITA()       fopen(PATH_ARQUIVO_PAGINAS, "rb+")

/** Operações no arquivo temporário **/
#define ABRIR_ARQUIVO_TMP_LEITURA()                   fopen(PATH_ARQUIVO_TEMP, "rb")
#define ABRIR_ARQUIVO_TMP_ESCRITA()                   fopen(PATH_ARQUIVO_TEMP, "wb")
#define ABRIR_ARQUIVO_TMP_LEITURA_ESCRITA()           fopen(PATH_ARQUIVO_TEMP, "rb+")

/** Operações de tamanho */
#define TAMANHO_INDICE()                              sizeof(NoIndice)
#define TAMANHO_PAGINA()                              sizeof(PaginaRegistros)
#define TAMANHO_CABECALHO_INDICE()                    sizeof(CabecalhoNosIndices)
#define TAMANHO_CABECALHO_PAGINAS()                   sizeof(CabecalhoPaginasRegistros)

// --------------- Enums --------------- //

/**
 * TiposArquivo - Tipos de arquivo que podem ser utilizados
 */
typedef enum {
    ARQUIVO_INDICE = 0,
    ARQUIVO_PAGINAS,
    ARQUIVO_TMP
} TiposArquivo;

// --------- Funções Utilitárias -------- //

/**
 * Remove caracteres estranhos depois do primeiro caractere \0
 * @param str string a ser corrigida
 * @param tamanho tamanho da string
 */
void removeCaracteresEstranhosString(char *str, int tamanho);

/**
 * Compara inteiros para ordenação crescente
 *
 * @param a Valor a
 * @param b Valor b
 * @return Retorna 1 quando (a > b), 0 quando (a == b) e -1 quando (a < b)
 */
int cmpInteiro(const void *a_ptr, const void *b_ptr);

/**
 * Obtém o tamanho de um arquivo binário.
 *
 * @param tipo_arquivo Tipo do arquivo a ser lido
 * @return Tamanho do arquivo em bytes
 */
long tamanhoArquivo(TiposArquivo tipo_arquivo);

#endif //UTILS_H
