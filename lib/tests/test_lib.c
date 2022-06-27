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
