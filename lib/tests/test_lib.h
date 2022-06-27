#ifndef TEST_LIB_H
#define TEST_LIB_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "../registro.h"

/**
 * Inicializa um arquivo de teste, setando algumas propriedades necessárias para o correto funcionamento
 */
#define INICIA_TEST_FILE() \
    const char *PATH_ARQUIVO_INDICES = "test_trabalho.indices.dat"; \
    const char *PATH_ARQUIVO_PAGINAS = "test_trabalho.paginas.dat"; \
    const char *PATH_ARQUIVO_TEMP    = "test_trabalho.tmp.dat";

/**
 * Copia string para buffer
 *
 * @param dest Buffer de destino
 * @param ori String de origem
 * @param tam Tamanho
 */
void copyStrToBuffer(char *dest, const char *ori, size_t tam);

/**
 * Limpa um buffer, isto é troca todos os caracteres destes por '\0'
 *
 * @param dest Buffer que será modificado
 * @param tam Tamanho do buffer
 */
void cleanBuffer(char *dest, int tam);

/**
 * Verifica se a string possui caracteres estranhos após o \0
 *
 * @param buf Buffer ou string a ser validada
 * @param tam_buf tamanho do buffer
 * @param exp_str String a ser comparada
 * @return Retorna true se a string comparada é válida e não possui caracteres estranhos após o final
 */
bool assertBufferStr(char *buf, size_t tam_buf, const char *exp_str);


/**
 * Le um registro de formato de saida padrão do arquivo
 *
 * @param stdout_file Arquivo de saída
 * @return Retorna um registro obtido através da impressão num arquivo
 */
Registro leRegistroStdoutFile(FILE *stdout_file);

/**
 * Verifica se os buffers do registro lido contem lixo
 *
 * @param registro_lido Registro lido do arquivo
 * @param registro_comparar Registro a ser comparado
 * @return True se os buffers do arquivo são semelhantes ao registro e não contém lixo
 */
bool validaBuffersRegistro(Registro *registro_lido, Registro *registro_comparar);

#endif //TEST_LIB_H
