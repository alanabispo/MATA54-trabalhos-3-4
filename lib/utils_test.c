#include "tests/test_lib.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "utils.h"

// ---------------- Init --------------- //

INICIA_TEST_FILE();

// ---------------- Mocks -------------- //

char str_vazia[1];

char str_completa[5];
const char exp_str_completa[] = "casa";

// --------------- Testes -------------- //

bool itDeveAceitarStringVazia(){
    strcpy(str_vazia, "");
    removeCaracteresEstranhosString(str_vazia, 1);

    if(str_vazia[0] == '\0'){
        return true;
    }
    else {
        return false;
    }
}

bool itDeveManterStringCompleta(){
    strcpy(str_completa, "casa");
    removeCaracteresEstranhosString(str_completa, 4);

    if(strncmp(str_completa, exp_str_completa, 4) == 0){
        return true;
    }

    return false;

}

bool testRemoveCaracteresEstranhosString() {
    assert(itDeveAceitarStringVazia()); // String de 1 caractere deve manter o mesmo
    assert(itDeveManterStringCompleta()); // Uma string terminada em \0 não deve alterar nenhum valor
    // Uma string com 1 caractere depois do \0 diferente do \0 deve ser removido
    // Uma string com 2 caracteres \0 válidos deve remover todos os caracteres depois do primeiro \0

    return true;
}

// --------------- Runner -------------- //

int main() {
    assert(testRemoveCaracteresEstranhosString());

    return EXIT_SUCCESS;
}