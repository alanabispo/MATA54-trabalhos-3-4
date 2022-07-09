#include "tests/test_lib.h"
#include "tests/common.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "constantes.h"
#include "arvore_2d.h"
#include "registro.h"
#include "pagina.h"
#include "utils.h"

// ---------------- Init --------------- //

INICIA_TEST_FILE();

// ---------------- Mocks -------------- //

const char stdout_filename[] = "out.txt";

MOCKS_REGISTRO();

PaginaRegistros pagina_a;
PaginaRegistros pagina_b;
PaginaRegistros pagina_c;

// -------------- Helpers -------------- //

// Inicializa as páginas
void initPaginas() {
    pagina_a = newPaginaRegistros();
    pagina_a.prox_pagina = 2;
    pagina_a.n_registros = 3;
    pagina_a.registros[0] = registro_a;
    pagina_b.registros[0] = registro_b;
    pagina_c.registros[0] = registro_c;

    pagina_b = newPaginaRegistros();
    pagina_b.n_registros = 1;
    pagina_b.registros[0] = registro_d;

    pagina_c = newPaginaRegistros();
    pagina_c.n_registros = 1;
    pagina_c.registros[0] = registro_e;
}

// --------------- Testes -------------- //

// Deve criar noIndice vazio
bool itDeveCriarNoIndiceVazio() {
    NoIndice indice = newNoIndice();

    assert(indice.direita == APONTADOR_VAZIO);
    assert(indice.esquerda == APONTADOR_VAZIO);

    assert(indice.ano == ANO_VAZIO);
    assert(indice.nivel == NIVEL_VAZIO);

    assert(indice.tipo == NO_VAZIO);
    assert(indice.tipo_direita == APONTAMENTO_VAZIO);
    assert(indice.tipo_esquerda == APONTAMENTO_VAZIO);

    return assertBufferStr(indice.nome_autor, TAMANHO_MAX_STRING, "");
}

// Testa newNoIndice()
bool testNewNoIndice() {
    assert(itDeveCriarNoIndiceVazio());

    return true;
}

// Deve criar cabecalho nos indices vazio
bool itDeveCriarCabecalhoNosIndicesVazio() {
    CabecalhoNosIndices cabecalho = newCabecalhoNosIndices();

    return cabecalho.n_indices == 0;
}

// Testa newCabecalhoNosIndices()
bool testNewCabecalhoNosIndices() {
    assert(itDeveCriarCabecalhoNosIndicesVazio());

    return true;
}

// Deve criar arquivo de indices
bool itDeveCriarArquivoIndices() {
    criarArquivoIndices();

    assert(tamanhoArquivo(ARQUIVO_INDICE) == TAMANHO_INDICE());

    FILE *arquivo = ABRIR_ARQUIVO_INDICES_LEITURA();
    assert(arquivo != NULL);

    NoIndice no_raiz = newNoIndice();
    fread(&no_raiz, TAMANHO_INDICE(), 1, arquivo);
    fclose(arquivo);

    assert(no_raiz.nivel == NIVEL_RAIZ);

    assert(no_raiz.direita == APONTADOR_VAZIO);
    assert(no_raiz.esquerda == APONTADOR_VAZIO);
    assert(no_raiz.ano == ANO_VAZIO);

    assert(no_raiz.tipo == NO_VAZIO);
    assert(no_raiz.tipo_direita == APONTAMENTO_VAZIO);
    assert(no_raiz.tipo_esquerda == APONTAMENTO_VAZIO);

    removeArquivoIndices();

    return true;
}

// Testa criarArquivoIndices()
bool testCriarArquivoIndices() {
    assert(itDeveCriarArquivoIndices());

    return true;
}

// Deve remover arquivo de indices
bool itDeveRemoverArquivoIndices() {
    criarArquivoIndices();

    FILE *arquivo_indices = ABRIR_ARQUIVO_INDICES_LEITURA();
    assert(arquivo_indices != NULL);
    fclose(arquivo_indices);

    removeArquivoIndices();
    arquivo_indices = ABRIR_ARQUIVO_INDICES_LEITURA();
    assert(arquivo_indices == NULL);

    return true;
}

// Testa removeArquivoIndices()
bool testRemoveArquivoIndices() {
    assert(itDeveRemoverArquivoIndices());

    return true;
}

// Deve ler cabecalho de indices vazio
bool itDeveLeCabecalhoIndicesVazio() {
    criarArquivoIndices();

    CabecalhoNosIndices cabecalho = leCabecalhoIndices();
    assert(cabecalho.n_indices == 0);

    return true;
}

// Deve ler cabecalho de indices com valores
bool itDeveLeCabecalhoIndices() {
    CabecalhoNosIndices cabecalho = newCabecalhoNosIndices();
    cabecalho.n_indices = 1;

    FILE *arquivo = ABRIR_ARQUIVO_INDICES_ESCRITA();
    fwrite(&cabecalho, TAMANHO_CABECALHO_INDICE(), 1, arquivo);
    fclose(arquivo);

    cabecalho = leCabecalhoIndices();
    assert(cabecalho.n_indices == 1);

    return true;
}

// Testa leCabecalhoIndices()
bool testLeCabecalhoIndices() {
    assert(itDeveLeCabecalhoIndicesVazio());
    assert(itDeveLeCabecalhoIndices());

    return true;
}

// TODO Testa leIndice()
bool testLeIndice() {
    return false;
}

// TODO Testa gravaCabecalhoIndices()
bool testGravaCabecalhoIndices() {
    return false;
}

// TODO Testa gravaIndice()
bool testGravaIndice() {
    return false;
}

// TODO Testa comparaNivel()
bool testComparaNivel() {
    return false;
}

// TODO Testa copiaNomeAutorIndice()
bool testCopiaNomeAutorIndice() {
    return false;
}

// TODO Testa incluirIndice()
bool testIncluirIndice() {
    return false;
}

// TODO Testa salvaRegistroPagina()
bool testSalvaRegistroPagina() {
    return false;
}

// TODO Testa moveRegistrosTmpPaginas()
bool testMoveRegistrosTmpPaginas() {
    return false;
}

// TODO Testa imprimeIndices()
bool testImprimeIndices() {
    return false;
}

// Gera stdout para ser testado
void generateStdout() {
    // TODO
}

// Testa stdout gerados previamente
bool testGeneratedStdout() {
    FILE *stdout_file = fopen(stdout_filename, "r");

    assert(testImprimeIndices());

    fclose(stdout_file);

    return true;
}

// --------------- Runner -------------- //

int main(int argc, char *argv[]) {
    const char opt_generate_stdout[] = "--generate-stdout";
    const char opt_test_generated_stdout[] = "--test-generated-stdout";

    initPaginas();

    // Checa a quantidade de argumentos
    if (argc >= 2) {
        if (strcmp(argv[1], opt_generate_stdout) == 0) {
            generateStdout();
        } else if (strcmp(argv[1], opt_test_generated_stdout) == 0) {
            assert(testGeneratedStdout());
        } else {
            printf("ARGUMENTO INVALIDO: %s\n", argv[1]);
            return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
    }

    // Testes que não dependem de stdout
    assert(testNewNoIndice());
    assert(testNewCabecalhoNosIndices());
    assert(testCriarArquivoIndices());
    assert(testRemoveArquivoIndices());
    assert(testLeCabecalhoIndices());
    assert(testLeIndice());
    assert(testGravaCabecalhoIndices());
    assert(testGravaIndice());
    assert(testComparaNivel());
    assert(testCopiaNomeAutorIndice());
    assert(testIncluirIndice());
    assert(testSalvaRegistroPagina());
    assert(testMoveRegistrosTmpPaginas());

    return EXIT_SUCCESS;
}