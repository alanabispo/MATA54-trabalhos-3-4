#ifndef TEST_LIB_H
#define TEST_LIB_H

#include <string.h>

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

#endif //TEST_LIB_H
