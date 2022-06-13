/**
 * Definição de um registro, estrutura responsável pelos metadados
 * dos arquivos de texto utilizados para busca.
 */

#ifndef REGISTRO_H
#define REGISTRO_H

#include <stdbool.h>

// ------------- Constantes ------------- //

#include "constantes.h"
extern const char *PATH_ARQUIVO_TEMP;

// --------------- Macros --------------- //

#define TAMANHO_REGISTRO() sizeof(Registro)

// ------------- Estruturas ------------- //

/**
 * Registro - Estrutura que armazena um registro da nossa estrutura
 */
typedef struct {
    char nome_autor[TAMANHO_MAX_STRING + 1]; // Adiciona +1 caractere para o \0
    char titulo[TAMANHO_MAX_STRING + 1];
    int ano;
    char nome_arquivo[TAMANHO_MAX_STRING + 1];
} Registro;

// --------------- Metodos -------------- //

/**
 * Cria um registro vazio
 *
 * @return Registro vazio.
 */
Registro newRegistro();

/**
 * Compara se dois registros são iguais
 *
 * @param a Registro A
 * @param b Registro B
 * @return Retorna o resultado da comparação dos dois registros
 */
bool equalsRegistro(Registro *a, Registro *b);

/**
 * Imprime um registro.
 *
 * @param registro Dados do registro a ser impresso.
 */
void imprimeRegistro(Registro *registro);

/**
 * Cria arquivo temporário para armazenamento de lista de registros
 */
void criarArquivoTmp();

/**
 * Adiciona registro temporariamente
 *
 * @param registro
 */
void adicionarRegistroTmp(int pos, Registro *registro);

/**
 * Retorna um registro de um arquivo temporário
 *
 * @param pos Posição a ser retornada
 * @return Registro lido a partir do arquivo temporário
 */
Registro leRegistroTmp(int pos);



#endif //REGISTRO_H
