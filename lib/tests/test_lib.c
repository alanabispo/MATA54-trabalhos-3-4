#pragma ide diagnostic ignored "readability-non-const-parameter"

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

// Verifica se a string possui caracteres estranhos após o \0
bool assertBufferStr(char *buf, size_t tam_buf, const char *exp_str) {
    size_t tam_exp = strlen(exp_str);

    int i;
    for (i = 0; i < tam_buf; i++) {
        if (
            (i < tam_exp && buf[i] != exp_str[i]) ||
            (i >= tam_exp && buf[i] != '\0')
        ) return false;
    }

    return true;
}

// Le um registro de formato de saida padrão do arquivo
Registro leRegistroStdoutFile(FILE *stdout_file) {
    Registro registro = newRegistro();

    fscanf(stdout_file, "%[^\n]s", registro.nome_autor);
    fscanf(stdout_file, "%*c");
    fscanf(stdout_file, "%[^\n]s", registro.titulo);
    fscanf(stdout_file, " %d", &registro.ano);
    fscanf(stdout_file, "%*c");
    fscanf(stdout_file, "%[^\n]s", registro.nome_arquivo);
    fscanf(stdout_file, "%*c");

    return registro;
}

// Lê página impressa em arquivo
ListaPaginaRegistros lePaginaStdoutFile(FILE *stdout_file) {
    int i;
    char str_buf[50];
    char pag_buf[10];

    // Inicializa a página
    ListaPaginaRegistros lista = { .tamanho = PAGINA_INEXISTENTE };
    for (i = 0; i < TAMANHO_PADRAO_LISTA_PAGINAS; i++) {
        lista.paginas[i] = newPaginaRegistros();
    }

    // Verifica linha de página
    fscanf(stdout_file, "%[^\n]s", str_buf);
    fscanf(stdout_file, "%*c");

    int id_pagina;
    sscanf(str_buf, "%s %d", pag_buf, &id_pagina);

    // Verifica se temos um id de página
    if (strcmp("pagina:", pag_buf) != 0) {
        return lista;
    }

    long tam_buf;

    // Incrementa e acessa página
    lista.tamanho = 1;
    PaginaRegistros *pagina = &lista.paginas[0];

    // Enquanto não encontrarmos o separador de registros continua
    do {
        // Verifica se há espaço na página atual ou move para próxima
        if (pagina->n_registros >= NREGSPORPAGINA) {
            pagina->prox_pagina = lista.tamanho;
            pagina = &lista.paginas[pagina->prox_pagina];
            lista.tamanho++;
        }

        // Lê um registro
        pagina->n_registros++;
        pagina->registros[pagina->n_registros - 1] = leRegistroStdoutFile(stdout_file);

        // Verifica se chegamos no separador final
        fscanf(stdout_file, "%[^\n]s", str_buf);
        tam_buf = (long)strlen(str_buf);
        fseek(stdout_file, -tam_buf, SEEK_CUR);
    } while (strcmp(str_buf, SEPARADOR_PAGINA) != 0 && lista.tamanho < TAMANHO_PADRAO_LISTA_PAGINAS);

    // Avança o separador no arquivo + '\n'
    fseek(stdout_file, tam_buf + 1, SEEK_CUR);

    return lista;
}

// Verifica se os buffers do registro lido contem lixo
bool validaBuffersRegistro(Registro *registro_lido, Registro *registro_comparar) {
    return assertBufferStr(registro_lido->nome_autor, TAMANHO_MAX_STRING + 1, registro_comparar->nome_autor) &&
           assertBufferStr(registro_lido->nome_arquivo, TAMANHO_MAX_STRING + 1, registro_comparar->nome_arquivo) &&
           assertBufferStr(registro_lido->titulo, TAMANHO_MAX_STRING + 1, registro_comparar->titulo);
}