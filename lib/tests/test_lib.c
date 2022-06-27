#pragma ide diagnostic ignored "readability-non-const-parameter"
#include "test_lib.h"

#include <stdbool.h>
#include <string.h>

// Copia string para buffer
void copyStrToBuffer(char *dest, const char *ori, size_t tam) {
    int i;
    for (i = 0; i < tam; i++) {
        dest[i] = ori[i];
    }
}

// Limpa um buffer, isto é troca todos os caracteres destes por '\0'
void cleanBuffer(char *dest, int tam) {
    int i;
    for (i = 0; i < tam; i++) {
        dest[i] = '\0';
    }
}

// Verifica se a string possui caracteres estranhos após o \0
bool assertBufferStr(char *buf, size_t tam_buf, const char *exp_str) {
    size_t tam_exp = strlen(exp_str);

    int i;
    for (i = 0; i < tam_buf; i++) {
        if (
            (i < tam_exp && buf[i] != exp_str[i]) ||
            (i >= tam_exp && buf[i] != '\0')
        ) return false;
    }

    return true;
}

// Le um registro de formato de saida padrão do arquivo
Registro leRegistroStdoutFile(FILE *stdout_file) {
    Registro registro = newRegistro();

    fscanf(stdout_file, "%[^\n]s", registro.nome_autor);
    fscanf(stdout_file, "%*c");
    fscanf(stdout_file, "%[^\n]s", registro.titulo);
    fscanf(stdout_file, " %d", &registro.ano);
    fscanf(stdout_file, "%*c");
    fscanf(stdout_file, "%[^\n]s", registro.nome_arquivo);
    fscanf(stdout_file, "%*c");

    return registro;
}

// Verifica se os buffers do registro lido contem lixo
bool validaBuffersRegistro(Registro *registro_lido, Registro *registro_comparar) {
    return assertBufferStr(registro_lido->nome_autor, TAMANHO_MAX_STRING + 1, registro_comparar->nome_autor) &&
           assertBufferStr(registro_lido->nome_arquivo, TAMANHO_MAX_STRING + 1, registro_comparar->nome_arquivo) &&
           assertBufferStr(registro_lido->titulo, TAMANHO_MAX_STRING + 1, registro_comparar->titulo);
}