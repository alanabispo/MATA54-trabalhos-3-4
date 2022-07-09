#include "tests/test_lib.h"
#include "tests/common.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "constantes.h"
#include "pagina.h"
#include "registro.h"
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
    pagina_a.registros[1] = registro_b;
    pagina_a.registros[2] = registro_c;

    pagina_b = newPaginaRegistros();
    pagina_b.n_registros = 1;
    pagina_b.registros[0] = registro_d;

    pagina_c = newPaginaRegistros();
    pagina_c.n_registros = 1;
    pagina_c.registros[0] = registro_e;
}

// Grava o mock das páginas sem o uso do method
void gravaMockPaginas() {
    CabecalhoPaginasRegistros cabecalho = newCabecalhoPaginasRegistros();
    cabecalho.total_paginas = 3;
    cabecalho.n_registros = 5;

    FILE *arquivo = ABRIR_ARQUIVO_PAGINAS_LEITURA_ESCRITA();
    fwrite(&cabecalho, TAMANHO_CABECALHO_PAGINAS(), 1, arquivo);
    fwrite(&pagina_a, TAMANHO_PAGINA(), 1, arquivo);
    fwrite(&pagina_b, TAMANHO_PAGINA(), 1, arquivo);
    fwrite(&pagina_c, TAMANHO_PAGINA(), 1, arquivo);
    fclose(arquivo);
}

// --------------- Testes -------------- //

// Deve criar um cabecalho paginas vazio
bool itDeveCriarCabecalhoPagina() {
    CabecalhoPaginasRegistros cabecalho = newCabecalhoPaginasRegistros();

    assert(cabecalho.n_registros == 0);
    assert(cabecalho.total_paginas == 0);
    assert(cabecalho.n_paginas_folhas == 0);

    return true;
}

// Testa newCabecalhoPaginasRegistros()
bool testNewCabecalhoPaginasRegistros() {
    assert(itDeveCriarCabecalhoPagina());

    return true;
}

// Deve criar uma página de registros
bool itDeveCriarPaginaRegistros() {
    PaginaRegistros pagina = newPaginaRegistros();

    assert(pagina.n_registros == 0);
    assert(pagina.prox_pagina == APONTADOR_VAZIO);

    int i;
    for (i = 0; i < NREGSPORPAGINA; i++) {
        assert(pagina.registros[i].ano == ANO_VAZIO);
        assert(validaBuffersRegistro(&pagina.registros[i], &registro_vazio));
    }

    return true;
}

// Testa newPaginaRegistros()
bool testNewPaginaRegistros() {
    assert(itDeveCriarPaginaRegistros());

    return true;
}

// Deve criar arquivo de páginas
bool itDeveCiarArquivoPaginas() {
    criarArquivoPaginas();

    FILE *arquivo = fopen(PATH_ARQUIVO_PAGINAS, "r");

    assert(arquivo != NULL);
    assert(fseek(arquivo, 0, SEEK_END) != -1);

    size_t tamanho = ftell(arquivo);
    fclose(arquivo);

    removeArquivoPaginas();

    return tamanho == TAMANHO_CABECALHO_PAGINAS();
}

// Testa criarArquivoPaginas()
bool testCriarArquivoPaginas() {
    assert(itDeveCiarArquivoPaginas());

    return true;
}

// Deve remover arquivo de páginas
bool itDeveRemoverArquivoPaginas() {
    criarArquivoPaginas();

    FILE *arquivo_paginas = ABRIR_ARQUIVO_PAGINAS_LEITURA();
    assert(arquivo_paginas != NULL);
    fclose(arquivo_paginas);

    removeArquivoPaginas();
    arquivo_paginas = ABRIR_ARQUIVO_PAGINAS_LEITURA();
    assert(arquivo_paginas == NULL);

    return true;
}

// Testa removeArquivoPaginas()
bool testRemoveArquivoPaginas() {
    assert(itDeveRemoverArquivoPaginas());

    return true;
}

// Deve ler cabecalho vazio
bool itDeveLerCabecalhoVazio() {
    criarArquivoPaginas();
    CabecalhoPaginasRegistros cabecalho = leCabecalhoPaginasRegistros();
    removeArquivoPaginas();

    assert(cabecalho.n_registros == 0);
    assert(cabecalho.total_paginas == 0);
    assert(cabecalho.n_paginas_folhas == 0);

    return true;
}

// Testa leCabecalhoPaginasRegistros()
bool testLeCabecalhoPaginasRegistros() {
    assert(itDeveLerCabecalhoVazio());

    return true;
}

// Deve gravar cabecalho vazio
bool itDeveGravarCabecalhoVazio() {
    criarArquivoPaginas();

    CabecalhoPaginasRegistros cabecalho = newCabecalhoPaginasRegistros();
    gravaCabecalhoPaginas(&cabecalho);

    CabecalhoPaginasRegistros cabecalho_disco = leCabecalhoPaginasRegistros();
    assert(cabecalho.n_paginas_folhas == cabecalho_disco.n_paginas_folhas);
    assert(cabecalho.total_paginas == cabecalho_disco.total_paginas);
    assert(cabecalho.n_registros == cabecalho_disco.n_registros);

    return true;
}

// Deve gravar cabecalho
bool itDeveGravarCabecalho() {
    criarArquivoPaginas();

    CabecalhoPaginasRegistros cabecalho = newCabecalhoPaginasRegistros();
    cabecalho.n_registros = 1;
    cabecalho.total_paginas = 2;
    cabecalho.n_paginas_folhas = 3;
    gravaCabecalhoPaginas(&cabecalho);

    CabecalhoPaginasRegistros cabecalho_disco = leCabecalhoPaginasRegistros();
    assert(cabecalho.n_paginas_folhas == cabecalho_disco.n_paginas_folhas);
    assert(cabecalho.total_paginas == cabecalho_disco.total_paginas);
    assert(cabecalho.n_registros == cabecalho_disco.n_registros);

    return true;
}

// Testa gravaCabecalhoPaginas()
bool testGravaCabecalhoPaginas() {
    assert(itDeveGravarCabecalhoVazio());
    assert(itDeveGravarCabecalho());

    return true;
}

// Deve retornar paginas diferentes
bool itDeveRetornarPaginasDiferentes() {
    return !equalsPagina(&pagina_a, &pagina_b);
}

// Deve retornar paginas iguais
bool itDeveRetornarPaginasIguais() {
    return equalsPagina(&pagina_a, &pagina_a);
}

// Testa equalsPagina()
bool testEqualsPagina() {
    assert(itDeveRetornarPaginasDiferentes());
    assert(itDeveRetornarPaginasIguais());

    return true;
}

// Deve indicar erro caso a página não exista
bool itDeveIndicarErroPaginaInexistente() {
    criarArquivoPaginas();

    PaginaRegistros pagina = lePagina(0);

    removeArquivoPaginas();

    assert(pagina.n_registros == PAGINA_INEXISTENTE);

    return true;
}

// Deve ler uma página
bool itDeveLerPagina() {
    criarArquivoPaginas();

    CabecalhoPaginasRegistros cabecalho = newCabecalhoPaginasRegistros();
    cabecalho.total_paginas = 1;
    cabecalho.n_registros = 1;

    FILE *arquivo = ABRIR_ARQUIVO_PAGINAS_LEITURA_ESCRITA();
    fwrite(&cabecalho, TAMANHO_CABECALHO_PAGINAS(), 1, arquivo);
    fwrite(&pagina_b, TAMANHO_PAGINA(), 1, arquivo);
    fclose(arquivo);

    PaginaRegistros pagina = lePagina(0);
    assert(equalsPagina(&pagina, &pagina_b));

    removeArquivoPaginas();

    return true;
}

// Deve ler múltiplas páginas
bool itDeveLerMultiplasPaginas() {
    criarArquivoPaginas();

    gravaMockPaginas();

    PaginaRegistros pag_a = lePagina(0);
    PaginaRegistros pag_b = lePagina(1);
    PaginaRegistros pag_c = lePagina(2);

    removeArquivoPaginas();

    assert(equalsPagina(&pag_a, &pagina_a));
    assert(equalsPagina(&pag_b, &pagina_b));
    assert(equalsPagina(&pag_c, &pagina_c));

    return true;
}

// Deve ler chain (corrente) de páginas
bool itDeveLerChainPaginas() {
    criarArquivoPaginas();

    gravaMockPaginas();

    PaginaRegistros pag_a = lePagina(0);
    PaginaRegistros pag_c = lePagina(pag_a.prox_pagina);

    removeArquivoPaginas();

    assert(equalsPagina(&pag_a, &pagina_a));
    assert(equalsPagina(&pag_c, &pagina_c));

    return true;
}

// Testa lePagina()
bool testLePagina() {
    assert(itDeveIndicarErroPaginaInexistente());
    assert(itDeveLerPagina());
    assert(itDeveLerMultiplasPaginas());
    assert(itDeveLerChainPaginas());

    return true;
}

// Deve gravar uma página
bool itDeveGravarPagina() {
    criarArquivoPaginas();

    CabecalhoPaginasRegistros cabecalho = {
            .total_paginas = 1,
            .n_registros = 1,
            .n_paginas_folhas = 0
    };
    gravaCabecalhoPaginas(&cabecalho);
    gravaPagina(0, &pagina_b);

    cabecalho = leCabecalhoPaginasRegistros();
    PaginaRegistros pagina = lePagina(0);

    removeArquivoPaginas();

    assert(pagina.n_registros != PAGINA_INEXISTENTE);
    assert(cabecalho.n_registros == 1);
    assert(cabecalho.total_paginas == 1);
    assert(equalsPagina(&pagina, &pagina_b));

    return true;
}

// Deve gravar múltiplas páginas sem sobrescrever
bool itDeveGravarMultiplasPaginas() {
    criarArquivoPaginas();

    CabecalhoPaginasRegistros cabecalho = {
        .total_paginas = 3,
        .n_registros = 5,
        .n_paginas_folhas = 0
    };
    gravaCabecalhoPaginas(&cabecalho);

    gravaPagina(0, &pagina_a);
    gravaPagina(1, &pagina_b);
    gravaPagina(2, &pagina_c);

    cabecalho = leCabecalhoPaginasRegistros();
    PaginaRegistros pag_a = lePagina(0);
    PaginaRegistros pag_b = lePagina(1);
    PaginaRegistros pag_c = lePagina(2);

    removeArquivoPaginas();

    assert(pag_a.n_registros != PAGINA_INEXISTENTE);
    assert(pag_b.n_registros != PAGINA_INEXISTENTE);
    assert(pag_c.n_registros != PAGINA_INEXISTENTE);

    assert(cabecalho.n_registros == 5);
    assert(cabecalho.total_paginas == 3);

    assert(equalsPagina(&pag_a, &pagina_a));
    assert(equalsPagina(&pag_b, &pagina_b));
    assert(equalsPagina(&pag_c, &pagina_c));

    return true;
}

// Deve sobrescrever paginas se gravar no mesmo indice
bool itDeveSobrescreverPaginas() {
    criarArquivoPaginas();

    CabecalhoPaginasRegistros cabecalho = {
            .total_paginas = 1,
            .n_registros = 1,
            .n_paginas_folhas = 0
    };
    gravaCabecalhoPaginas(&cabecalho);

    gravaPagina(0, &pagina_b);
    gravaPagina(0, &pagina_c);

    cabecalho = leCabecalhoPaginasRegistros();
    PaginaRegistros pagina = lePagina(0);

    assert(pagina.n_registros != PAGINA_INEXISTENTE);
    assert(cabecalho.n_registros == 1);
    assert(cabecalho.total_paginas == 1);
    assert(equalsPagina(&pagina, &pagina_c));

    return true;
}

// Testa gravaPagina()
bool testGravaPagina() {
    assert(itDeveGravarPagina());
    assert(itDeveGravarMultiplasPaginas());
    assert(itDeveSobrescreverPaginas());

    return true;
}

// Gera stdout para ser testado
void generateStdout() {
    criarArquivoPaginas();
    gravaMockPaginas();

    imprimePagina(0);
    printf(SEPARADOR_PAGINA "\n");

    imprimePagina(1);
    printf(SEPARADOR_PAGINA "\n");

    imprimePagina(2);
    printf(SEPARADOR_PAGINA "\n");

    removeArquivoPaginas();
}

// Deve imprimir corretamente pagina 0
bool itDeveImprimirCorretamentePagina0(FILE *stdout_file) {
    ListaPaginaRegistros lista = lePaginaStdoutFile(stdout_file);

    // A página 0 é ligada a página 2 logo temos duas páginas
    assert(lista.tamanho == 2);
    assert(equalsPagina(&lista.paginas[0], &pagina_a));
    assert(equalsPagina(&lista.paginas[1], &pagina_c));

    return true;
}

// Deve imprimir corretamente pagina 1
bool itDeveImprimirCorretamentePagina1(FILE *stdout_file) {
    ListaPaginaRegistros lista = lePaginaStdoutFile(stdout_file);

    assert(lista.tamanho == 1);
    assert(equalsPagina(&lista.paginas[0], &pagina_b));

    return true;
}

// Deve imprimir corretamente pagina 2
bool itDeveImprimirCorretamentePagina2(FILE *stdout_file) {
    ListaPaginaRegistros lista = lePaginaStdoutFile(stdout_file);

    assert(lista.tamanho == 1);
    assert(equalsPagina(&lista.paginas[0], &pagina_c));

    return true;
}

// Testa imprimePagina()
bool testImprimePagina(FILE *stdout_file) {
    assert(itDeveImprimirCorretamentePagina0(stdout_file));
    assert(itDeveImprimirCorretamentePagina1(stdout_file));
    assert(itDeveImprimirCorretamentePagina2(stdout_file));

    return true;
}

// Testa stdout gerados previamente
bool testGeneratedStdout() {
    FILE *stdout_file = fopen(stdout_filename, "r");

    assert(testImprimePagina(stdout_file));

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
    assert(testNewCabecalhoPaginasRegistros());
    assert(testNewPaginaRegistros());
    assert(testCriarArquivoPaginas());
    assert(testRemoveArquivoPaginas());
    assert(testLeCabecalhoPaginasRegistros());
    assert(testGravaCabecalhoPaginas());
    assert(testEqualsPagina());
    assert(testLePagina());
    assert(testGravaPagina());

    return EXIT_SUCCESS;
}