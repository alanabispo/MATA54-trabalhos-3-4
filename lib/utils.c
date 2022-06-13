#include "utils.h"

#include <stdio.h>
#include <stddef.h>
#include <string.h>

// Remove caracteres estranhos depois do primeiro caractere \0
void removeCaracteresEstranhosString(char *str) {
    size_t i;
    size_t tamanho_string = strnlen(str, TAMANHO_MAX_STRING + 1);

    for (i = tamanho_string; i < (TAMANHO_MAX_STRING + 1); i++) {
        str[i] = '\0';
    }
}

// Compara inteiros para ordenação crescente
int cmpInteiro(const void *a_ptr, const void *b_ptr) {
    int a = *(const int *)a_ptr;
    int b = *(const int *)b_ptr;

    if (a > b) {
        return 1;
    }

    if (b < a) {
        return -1;
    }

    return 0;
}

// Obtém o tamanho de um arquivo binário
long tamanhoArquivo(TiposArquivo tipo_arquivo) {
    long tamanho;
    FILE* arquivo;

    switch (tipo_arquivo) {
        case ARQUIVO_INDICE:
            arquivo = ABRIR_ARQUIVO_INDICES_LEITURA();
            break;
        case ARQUIVO_PAGINAS:
            arquivo = ABRIR_ARQUIVO_PAGINAS_LEITURA();
            break;
        case ARQUIVO_TMP:
            arquivo = ABRIR_ARQUIVO_TMP_LEITURA();
            break;
        default:
            return ARQUIVO_INEXISTENTE;
    }

    // Falha ao abrir arquivo
    if (arquivo == NULL) {
        return ARQUIVO_INEXISTENTE;
    }

    // Percorre o arquivo até o final
    if (fseek(arquivo, 0, SEEK_END)) {
        fclose(arquivo);
        return ARQUIVO_TAMANHO_INVALIDO;
    }

    // Informa onde o ponteiro de leitura parou (esse é o tamanho do arquivo)
    tamanho = ftell(arquivo);
    fclose(arquivo);

    return tamanho;
}