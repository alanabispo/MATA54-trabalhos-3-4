#include "tests/test_lib.h"
#include "tests/common.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "constantes.h"
#include "registro.h"
#include "utils.h"

// ---------------- Init --------------- //

INICIA_TEST_FILE();

// ---------------- Mocks -------------- //

const char stdout_filename[] = "out.txt";

MOCKS_REGISTRO();

// -------------- Helpers -------------- //

// Remove arquivo tmp
void removeArquivoTmp() {
    remove(PATH_ARQUIVO_TEMP);
}

// --------------- Testes -------------- //

// Deve criar um registro vazio
bool itDeveCriarRegistroVazio() {
    Registro registro = newRegistro();

    assert(validaBuffersRegistro(&registro, &registro_vazio));
    assert(registro.ano == ANO_VAZIO);

    return true;
}

// Testes newRegistro()
bool testNewRegistro() {
    assert(itDeveCriarRegistroVazio());

    return true;
}

// Deve marcar registros vazios como iguais
bool itDeveCompararRegistrosVazia() {
    Registro registro = newRegistro();

    return equalsRegistro(&registro, &registro_vazio);
}

// Deve retornar que os registros são iguais
bool itDeveRetornarRegistrosIguais() {
    Registro reg_a = {
        .ano = 2000,
        .nome_autor = "teste",
        .nome_arquivo = "a.txt",
        .titulo = "teste_titulo"
    };

    Registro reg_b = {
        .ano = 2000,
        .nome_autor = "teste",
        .nome_arquivo = "a.txt",
        .titulo = "teste_titulo"
    };

    return equalsRegistro(&reg_a, &reg_b);
}

// Deve retornar que os registros são diferentes
bool itDeveRetornarRegistrosDiferentesAno() {
    Registro reg_a = {
            .ano = 2000,
            .nome_autor = "teste",
            .nome_arquivo = "a.txt",
            .titulo = "teste_titulo"
    };

    Registro reg_b = {
            .ano = 2022,
            .nome_autor = "teste",
            .nome_arquivo = "a.txt",
            .titulo = "teste_titulo"
    };

    return !equalsRegistro(&reg_a, &reg_b);
}


// Deve retornar que os registros são diferentes
bool itDeveRetornarRegistrosDiferentesNomeAutor() {
    Registro reg_a = {
            .ano = 2000,
            .nome_autor = "teste",
            .nome_arquivo = "a.txt",
            .titulo = "teste_titulo"
    };

    Registro reg_b = {
            .ano = 2020,
            .nome_autor = "teste2",
            .nome_arquivo = "a.txt",
            .titulo = "teste_titulo"
    };

    return !equalsRegistro(&reg_a, &reg_b);
}

// Deve retornar que os registros são diferentes
bool itDeveRetornarRegistrosDiferentesNomeArquivo() {
    Registro reg_a = {
            .ano = 2000,
            .nome_autor = "teste",
            .nome_arquivo = "a.txt",
            .titulo = "teste_titulo"
    };

    Registro reg_b = {
            .ano = 2020,
            .nome_autor = "teste",
            .nome_arquivo = "b.txt",
            .titulo = "teste_titulo"
    };

    return !equalsRegistro(&reg_a, &reg_b);
}

// Deve retornar que os registros são diferentes
bool itDeveRetornarRegistrosDiferentesTitulo() {
    Registro reg_a = {
            .ano = 2000,
            .nome_autor = "teste",
            .nome_arquivo = "a.txt",
            .titulo = "teste_titulo"
    };

    Registro reg_b = {
            .ano = 2020,
            .nome_autor = "teste",
            .nome_arquivo = "a.txt",
            .titulo = "teste_titulo_2"
    };

    return !equalsRegistro(&reg_a, &reg_b);
}

// Testes equalsRegistro()
bool testEqualsRegistro() {
    assert(itDeveCompararRegistrosVazia());
    assert(itDeveRetornarRegistrosIguais());
    assert(itDeveRetornarRegistrosDiferentesAno());
    assert(itDeveRetornarRegistrosDiferentesNomeArquivo());
    assert(itDeveRetornarRegistrosDiferentesNomeAutor());
    assert(itDeveRetornarRegistrosDiferentesTitulo());

    return true;
}

// Deve criar um arquivo temporário
bool itDeveCriarArquivoTmp() {
    criarArquivoTmp();

    FILE *arquivo_tmp = fopen(PATH_ARQUIVO_TEMP, "r");
    fclose(arquivo_tmp);
    removeArquivoTmp();

    return arquivo_tmp != NULL;
}

// Testa criarArquivoTmp()
bool testCriarArquivoTmp() {
    assert(itDeveCriarArquivoTmp());

    removeArquivoTmp();

    return true;
}

// Deve adicionar um registro mesmo que vazio
bool itDeveAdicionarRegistroVazio() {
    criarArquivoTmp();

    size_t tam_arquivo = tamanhoArquivo(ARQUIVO_TMP);

    assert(tam_arquivo == 0);

    adicionarRegistroTmp(0, &registro_vazio);

    tam_arquivo = tamanhoArquivo(ARQUIVO_TMP);

    Registro registro = newRegistro();
    FILE *arquivo = ABRIR_ARQUIVO_TMP_LEITURA_ESCRITA();
    fread(&registro, TAMANHO_REGISTRO(), 1, arquivo);
    fclose(arquivo);
    removeArquivoTmp();

    assert(tam_arquivo == TAMANHO_REGISTRO());
    assert(equalsRegistro(&registro_vazio, &registro));
    assert(validaBuffersRegistro(&registro, &registro_vazio));

    return true;
}

// Deve permitir a adição de um registro
bool itDeveAdicionarUmRegistro() {
    criarArquivoTmp();

    size_t tam_arquivo = tamanhoArquivo(ARQUIVO_TMP);

    assert(tam_arquivo == 0);

    adicionarRegistroTmp(0, &registro_a);

    tam_arquivo = tamanhoArquivo(ARQUIVO_TMP);

    Registro registro = newRegistro();
    FILE *arquivo = ABRIR_ARQUIVO_TMP_LEITURA_ESCRITA();
    fread(&registro, TAMANHO_REGISTRO(), 1, arquivo);
    fclose(arquivo);
    removeArquivoTmp();

    assert(tam_arquivo == TAMANHO_REGISTRO());
    assert(equalsRegistro(&registro_a, &registro));
    assert(validaBuffersRegistro(&registro, &registro_a));

    return true;
}

// Deve permitir a adição de múltiplos registros
bool itDeveAdicionarMultiplosRegistros() {
    criarArquivoTmp();

    size_t tam_arquivo = tamanhoArquivo(ARQUIVO_TMP);

    assert(tam_arquivo == 0);

    adicionarRegistroTmp(0, &registro_a);
    adicionarRegistroTmp(1, &registro_b);
    adicionarRegistroTmp(2, &registro_c);
    adicionarRegistroTmp(3, &registro_d);

    tam_arquivo = tamanhoArquivo(ARQUIVO_TMP);

    Registro reg_a = newRegistro();
    Registro reg_b = newRegistro();
    Registro reg_c = newRegistro();
    Registro reg_d = newRegistro();

    FILE *arquivo = ABRIR_ARQUIVO_TMP_LEITURA_ESCRITA();
    fread(&reg_a, TAMANHO_REGISTRO(), 1, arquivo);
    fread(&reg_b, TAMANHO_REGISTRO(), 1, arquivo);
    fread(&reg_c, TAMANHO_REGISTRO(), 1, arquivo);
    fread(&reg_d, TAMANHO_REGISTRO(), 1, arquivo);
    fclose(arquivo);
    removeArquivoTmp();

    assert(tam_arquivo == (TAMANHO_REGISTRO() * 4));
    assert(equalsRegistro(&registro_a, &reg_a));
    assert(equalsRegistro(&registro_b, &reg_b));
    assert(equalsRegistro(&registro_c, &reg_c));
    assert(equalsRegistro(&registro_d, &reg_d));
    assert(validaBuffersRegistro(&reg_a, &registro_a));
    assert(validaBuffersRegistro(&reg_b, &registro_b));
    assert(validaBuffersRegistro(&reg_c, &registro_c));
    assert(validaBuffersRegistro(&reg_d, &registro_d));

    return true;
}

// Testa adicionarRegistroTmp()
bool testAdicionarRegistroTmp() {
    assert(itDeveAdicionarRegistroVazio());
    assert(itDeveAdicionarUmRegistro());
    assert(itDeveAdicionarMultiplosRegistros());

    return true;
}

// Deve indicar erro de registro inexistente
bool itDeveIndicarErroRegistroInexistente() {
    criarArquivoTmp();

    Registro registro = leRegistroTmp(0);

    removeArquivoTmp();

    return registro.ano == REGISTRO_INEXISTENTE;
}

// Deve ler um registro
bool itDeveLerRegistro() {
    criarArquivoTmp();

    adicionarRegistroTmp(0, &registro_a);
    Registro registro = leRegistroTmp(0);

    removeArquivoTmp();

    return equalsRegistro(&registro, &registro_a);
}

// Deve ler múltiplos registros sem alterar os outros já existentes
bool itDeveLerRegistrosSemAlterarOutros() {
    criarArquivoTmp();

    adicionarRegistroTmp(0, &registro_a);
    adicionarRegistroTmp(1, &registro_b);
    adicionarRegistroTmp(2, &registro_c);
    adicionarRegistroTmp(3, &registro_d);

    Registro reg_a = leRegistroTmp(0);
    Registro reg_b = leRegistroTmp(1);
    Registro reg_c = leRegistroTmp(2);
    Registro reg_d = leRegistroTmp(3);

    removeArquivoTmp();

    return equalsRegistro(&reg_a, &registro_a) &&
        equalsRegistro(&reg_b, &registro_b) &&
        equalsRegistro(&reg_c, &registro_c) &&
        equalsRegistro(&reg_d, &registro_d);
}

// Testa leRegistroTmp()
bool testLeRegistroTmp() {
    assert(itDeveIndicarErroRegistroInexistente());
    assert(itDeveLerRegistro());
    assert(itDeveLerRegistrosSemAlterarOutros());

    return true;
}

// Gera stdout para ser testado
void generateStdout() {
    imprimeRegistro(&registro_a);
    imprimeRegistro(&registro_b);
    imprimeRegistro(&registro_c);
    imprimeRegistro(&registro_d);
}

// Compara o registro a no arquivo
bool itDeveImprimirRegistroCorretamenteA(FILE *stdout_file) {
    Registro reg_arquivo = leRegistroStdoutFile(stdout_file);

    return equalsRegistro(&registro_a, &reg_arquivo);
}

// Compara o registro b no arquivo
bool itDeveImprimirRegistroCorretamenteB(FILE *stdout_file) {
    Registro reg_arquivo = leRegistroStdoutFile(stdout_file);

    return equalsRegistro(&registro_b, &reg_arquivo);
}

// Compara o registro c no arquivo
bool itDeveImprimirRegistroCorretamenteC(FILE *stdout_file) {
    Registro reg_arquivo = leRegistroStdoutFile(stdout_file);

    return equalsRegistro(&registro_c, &reg_arquivo);
}

// Compara o registro d no arquivo
bool itDeveImprimirRegistroCorretamenteD(FILE *stdout_file) {
    Registro reg_arquivo = leRegistroStdoutFile(stdout_file);

    return equalsRegistro(&registro_d, &reg_arquivo);
}

// Testa stdout gerados previamente
bool testGeneratedStdout() {
    FILE *stdout_file = fopen(stdout_filename, "r");

    assert(itDeveImprimirRegistroCorretamenteA(stdout_file));
    assert(itDeveImprimirRegistroCorretamenteB(stdout_file));
    assert(itDeveImprimirRegistroCorretamenteC(stdout_file));
    assert(itDeveImprimirRegistroCorretamenteD(stdout_file));

    fclose(stdout_file);

    return true;
}

// --------------- Runner -------------- //

int main(int argc, char *argv[]) {
    const char opt_generate_stdout[] = "--generate-stdout";
    const char opt_test_generated_stdout[] = "--test-generated-stdout";

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
    assert(testNewRegistro());
    assert(testEqualsRegistro());
    assert(testCriarArquivoTmp());
    assert(testAdicionarRegistroTmp());
    assert(testLeRegistroTmp());

    return EXIT_SUCCESS;
}