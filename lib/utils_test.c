#pragma ide diagnostic ignored "readability-non-const-parameter"
#include "tests/test_lib.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "utils.h"
#include "constantes.h"

// ---------------- Init --------------- //

INICIA_TEST_FILE();

// ---------------- Mocks -------------- //

#define STR_VAZIA ""

#define STR_2 "TEST"

#define STR_3 "TEST"
#define STR_3_COM_LIXO STR_3 "\0@!#"

#define STR_4_COM_LIXO "\0@#$%¨&*()"

#define TXT_100_BYTES \
    "Lorem ipsum dolor sit amet, consectetur adipiscing elit,\n" \
    "sed eiusmod tempor incididunt magna aliqua."

char str_buffer[TAMANHO_MAX_STRING + 1];
char str_exp_buffer[TAMANHO_MAX_STRING + 1];

// -------------- Helpers -------------- //

// Verifica se a string tem o formato desejado
bool assertBufferStr(char *buf, size_t tam_buf, const char *exp_str) {
    size_t tam_exp = strlen(exp_str);
    copyStrToBuffer(str_exp_buffer, exp_str, tam_exp);

    int i;
    for (i = 0; i < tam_buf; i++) {
        if (
            (i < tam_exp && buf[i] != exp_str[i]) ||
            (i >= tam_exp && buf[i] != '\0')
        ) return false;
    }

    return true;
}

// Grava string em formato binário no disco
void gravaStringBinariaDisco(const char *filename, const char *str) {
    FILE *arquivo = fopen(filename, "wb");
    fprintf(arquivo, "%s", str);
    fclose(arquivo);
}

// Remove arquivos de teste
void removeArquivosTeste() {
    remove(PATH_ARQUIVO_INDICES);
    remove(PATH_ARQUIVO_PAGINAS);
    remove(PATH_ARQUIVO_TEMP);
}

// --------------- Testes -------------- //

// Deve aceitar uma string vazia como input
bool itDeveAceitarStringVazia(){
    cleanBuffer(str_buffer, TAMANHO_MAX_STRING);
    copyStrToBuffer(str_buffer, STR_VAZIA, 1);

    removeCaracteresEstranhosString(str_buffer);

    return assertBufferStr(str_buffer, TAMANHO_MAX_STRING, STR_VAZIA);
}

// Deve manter string sem lixo
bool itDeveManterStringSemLixo(){
    cleanBuffer(str_buffer, TAMANHO_MAX_STRING);
    copyStrToBuffer(str_buffer, STR_2, 8);

    removeCaracteresEstranhosString(str_buffer);

    return assertBufferStr(str_buffer, TAMANHO_MAX_STRING, STR_2);
}

// Função de assert de buffer sem lixo deve funcionar sem lixo
bool itDeveTerSucessoBufferLimpoAssertBufferStr() {
    cleanBuffer(str_buffer, TAMANHO_MAX_STRING);
    copyStrToBuffer(str_buffer, STR_3, 8);

    return !assertBufferStr(str_buffer, TAMANHO_MAX_STRING, STR_3);
}


// Função de assert de buffer sem lixo deve falhar na presença de lixo
bool itDeveFalharAssertBufferStr() {
    cleanBuffer(str_buffer, TAMANHO_MAX_STRING);
    copyStrToBuffer(str_buffer, STR_3_COM_LIXO, 8);

    return !assertBufferStr(str_buffer, TAMANHO_MAX_STRING, STR_3);
}

// Deve remover lixo string
bool itDeveRemoverLixoString() {
    cleanBuffer(str_buffer, TAMANHO_MAX_STRING);
    copyStrToBuffer(str_buffer, STR_3_COM_LIXO, 8);

    removeCaracteresEstranhosString(str_buffer);

    return assertBufferStr(str_buffer, TAMANHO_MAX_STRING, STR_3);
}

// Deve remover lixo de string vazia
bool itDeveRemoverLixoStringVazia() {
    cleanBuffer(str_buffer, TAMANHO_MAX_STRING);
    copyStrToBuffer(str_buffer, STR_4_COM_LIXO, 10);

    removeCaracteresEstranhosString(str_buffer);

    return assertBufferStr(str_buffer, TAMANHO_MAX_STRING, STR_VAZIA);
}

// Testes removeCaracteresEstranhosString()
bool testRemoveCaracteresEstranhosString() {
    assert(itDeveAceitarStringVazia());
    assert(itDeveManterStringSemLixo());
    assert(itDeveTerSucessoBufferLimpoAssertBufferStr());
    assert(itDeveFalharAssertBufferStr());
    assert(itDeveRemoverLixoString());
    assert(itDeveRemoverLixoStringVazia());

    return true;
}

// Deve retornar 1 pois A > B
bool itDeveRetornar1() {
    const int a = 10;
    const int b = 5;

    return (cmpInteiro(&a, &b) == 1);
}

// Deve retornar 0 pois A == B
bool itDeveRetornar0() {
    const int a = 10;
    const int b = 10;

    return (cmpInteiro(&a, &b) == 0);
}

// Deve retornar -1 pois A < B
bool itDeveRetornarNegativo() {
    const int a = 10;
    const int b = 20;

    return (cmpInteiro(&a, &b) == -1);
}

// Testes cmpInteiro()
bool testCmpInteiro() {
    assert(itDeveRetornar1());
    assert(itDeveRetornar0());
    assert(itDeveRetornarNegativo());

    return true;
}

// Deve retornar ARQUIVO_INEXISTENTE
bool itDeveRetornarArquivoInexistente() {
    removeArquivosTeste();

    const long resArquivoPaginas = tamanhoArquivo(ARQUIVO_PAGINAS);
    const long resArquivoIndice = tamanhoArquivo(ARQUIVO_INDICE);
    const long resArquivoTmp = tamanhoArquivo(ARQUIVO_TMP);

    const long resArquivoInexistente = tamanhoArquivo(6);

    return resArquivoPaginas == ARQUIVO_INEXISTENTE &&
        resArquivoIndice == ARQUIVO_INEXISTENTE &&
        resArquivoTmp == ARQUIVO_INEXISTENTE &&
        resArquivoInexistente == ARQUIVO_INEXISTENTE;
}

// Testa a criação com arquivos de 100bytes
bool itDeveRetornar100BytesTamanho() {
    gravaStringBinariaDisco(PATH_ARQUIVO_PAGINAS, TXT_100_BYTES);
    gravaStringBinariaDisco(PATH_ARQUIVO_INDICES, TXT_100_BYTES);
    gravaStringBinariaDisco(PATH_ARQUIVO_TEMP, TXT_100_BYTES);

    const long resArquivoPaginas = tamanhoArquivo(ARQUIVO_PAGINAS);
    const long resArquivoIndice = tamanhoArquivo(ARQUIVO_INDICE);
    const long resArquivoTmp = tamanhoArquivo(ARQUIVO_TMP);

    return resArquivoPaginas == 100 &&
           resArquivoIndice == 100 &&
           resArquivoTmp == 100;
}

// Testes tamanhoArquivo()
bool testTamanhoArquivo() {
    assert(itDeveRetornarArquivoInexistente());
    assert(itDeveRetornar100BytesTamanho());

    return true;
}

// --------------- Runner -------------- //

int main() {
    assert(testRemoveCaracteresEstranhosString());
    assert(testCmpInteiro());
    assert(testTamanhoArquivo());

    return EXIT_SUCCESS;
}
