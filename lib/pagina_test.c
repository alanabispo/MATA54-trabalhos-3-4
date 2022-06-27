#include "tests/test_lib.h"

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

Registro registro_vazio = {
        .nome_autor = "",
        .ano = ANO_VAZIO,
        .nome_arquivo = "",
        .titulo = ""
};

Registro registro_a = {
        .titulo = "titulo_a",
        .nome_arquivo = "a.txt",
        .nome_autor = "teste",
        .ano = 2000
};
Registro registro_b = {
        .titulo = "titulo_b",
        .nome_arquivo = "b.txt",
        .nome_autor = "teste",
        .ano = 2001
};
Registro registro_c = {
        .titulo = "titulo c - a volta",
        .nome_arquivo = "c.txt",
        .nome_autor = "testinho",
        .ano = 2002
};
Registro registro_d = {
        .titulo = "maktub",
        .nome_arquivo = "ddd.txt",
        .nome_autor = "paulo coelho",
        .ano = 2003
};
Registro registro_e = {
        .titulo = "Meio do Caminho",
        .nome_arquivo = "eagora.txt",
        .nome_autor = "Carlos Drummond",
        .ano = 1967
};

PaginaRegistros pagina_vazia = {
        .registros = {
                {
                        .nome_autor = "",
                        .ano = ANO_VAZIO,
                        .nome_arquivo = "",
                        .titulo = ""
                },
                {
                        .nome_autor = "",
                        .ano = ANO_VAZIO,
                        .nome_arquivo = "",
                        .titulo = ""
                },
                {
                        .nome_autor = "",
                        .ano = ANO_VAZIO,
                        .nome_arquivo = "",
                        .titulo = ""
                }
        },
        .prox_pagina = REGISTRO_INEXISTENTE,
        .n_registros = 0
};

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

// Remove arquivo tmp
void removeArquivoPagina() {
    remove(PATH_ARQUIVO_PAGINAS);
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

    FILE *arquivo = fopen(PATH_ARQUIVO_TEMP, "r");

    if (fseek(arquivo, 0, SEEK_END)) {
        fclose(arquivo);
        return ARQUIVO_TAMANHO_INVALIDO;
    }

    size_t tamanho = ftell(arquivo);
    fclose(arquivo);

    removeArquivoPagina();

    return arquivo != NULL && tamanho == TAMANHO_CABECALHO_PAGINAS();
}

// Testa criarArquivoPaginas()
bool testCriarArquivoPaginas() {
    assert(itDeveCiarArquivoPaginas());

    return true;
}

// Deve ler cabecalho vazio
bool itDeveLerCabecalhoVazio() {
    criarArquivoPaginas();
    CabecalhoPaginasRegistros cabecalho = leCabecalhoPaginasRegistros();
    removeArquivoPagina();

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
    return true;
}

// Deve gravar cabecalho
bool itDeveGravarCabecalho() {
    return true;
}

// Test gravaCabecalhoPaginas()
bool testGravaCabecalhoPaginas() {
    assert(itDeveGravarCabecalhoVazio());
    assert(itDeveGravarCabecalho());

    return true;
}

// Gera stdout para ser testado
void generateStdout() {
    // TODO
}

// Testa stdout gerados previamente
bool testGeneratedStdout() {
    FILE *stdout_file = fopen(stdout_filename, "r");

    // TODO

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
//    assert(testNewCabecalhoPaginasRegistros());
//    assert(testNewPaginaRegistros());
//    assert(testCriarArquivoPaginas());
//    assert(testLeCabecalhoPaginasRegistros());
//    assert(testGravaCabecalhoPaginas());

    return EXIT_SUCCESS;
}