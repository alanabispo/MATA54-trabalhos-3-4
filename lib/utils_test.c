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

char str_vazia[] = "";

char str_completa[5] = "casa";
char exp_str_completa[5] = "casa";

// --------------- Testes -------------- //

bool itDeveAceitarStringVazia(){
    removeCaracteresEstranhosString(str_vazia);

    if(str_vazia[0] == '\0'){
        return true;
    }
    else {
        return false;
    }
}

bool itDeveManterStringCompleta(){
    removeCaracteresEstranhosString(str_completa);
    if(strncmp(str_completa, exp_str_completa, 4) == 0){
        return true;
    }

    return false;

}

bool testRemoveCaracteresEstranhosString() {
    assert(itDeveAceitarStringVazia());
    assert(itDeveManterStringCompleta());

    return true;
}

// --------------- Runner -------------- //

int main() {
    assert(testRemoveCaracteresEstranhosString());

    return EXIT_SUCCESS;
}