#include "registro.h"
#include "utils.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Cria um registro vazio
Registro newRegistro() {
    int i;
    Registro registro;

    // Zera propriedades
    registro.ano = ANO_VAZIO;

    // Zera vetores
    for (i = 0; i < (TAMANHO_MAX_STRING + 1); i++) {
        registro.titulo[i] = '\0';
        registro.nome_autor[i] = '\0';
        registro.nome_arquivo[i] = '\0';
    }

    return registro;
}

// Compara se dois registros são iguais
bool equalsRegistro(Registro *a, Registro *b) {
    return (
        a->ano == b->ano &&
        strncmp(a->nome_autor, b->nome_autor, TAMANHO_MAX_STRING + 1) == 0 &&
        strncmp(a->titulo, b->titulo, TAMANHO_MAX_STRING + 1) == 0 &&
        strncmp(a->nome_arquivo, b->nome_arquivo, TAMANHO_MAX_STRING + 1) == 0
    );
}

// Imprime um registro.
void imprimeRegistro(Registro *registro) {
    printf("%s\n%s\n%d\n%s\n", registro->nome_autor, registro->titulo, registro->ano, registro->nome_arquivo);
}

// Cria arquivo temporário para armazenamento de lista de registros
void criarArquivoTmp() {
    FILE *arquivo = ABRIR_ARQUIVO_TMP_ESCRITA();
    fclose(arquivo);
}

// Adiciona registro temporariamente
void adicionarRegistroTmp(int pos, Registro *registro) {
    FILE *arquivo = ABRIR_ARQUIVO_TMP_LEITURA_ESCRITA();

    // Avança até a posição e grava
    fseek(arquivo, TAMANHO_REGISTRO() * pos, SEEK_CUR);
    fwrite(registro, TAMANHO_REGISTRO(), 1, arquivo);

    fclose(arquivo);
}

// Retorna um registro de um arquivo temporário
Registro leRegistroTmp(int pos) {
    FILE *arquivo = ABRIR_ARQUIVO_TMP_LEITURA();
    Registro registro = newRegistro();

    // Avança até a posição e grava
    fseek(arquivo, TAMANHO_REGISTRO() * pos, SEEK_CUR);
    fread(&registro, TAMANHO_REGISTRO(), 1, arquivo);

    fclose(arquivo);

    return registro;
}