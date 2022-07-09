/**
 * Constantes comuns aos testes
 */

#ifndef COMMON_H
#define COMMON_H

/**
 * Mocks de registro
 */
#define MOCKS_REGISTRO() \
    Registro registro_vazio = { \
            .nome_autor = "", \
            .ano = ANO_VAZIO, \
            .nome_arquivo = "", \
            .titulo = "" \
    }; \
    Registro registro_a = { \
            .titulo = "titulo_a", \
            .nome_arquivo = "a.txt", \
            .nome_autor = "teste", \
            .ano = 2000 \
    }; \
    Registro registro_b = { \
            .titulo = "titulo_b",\
            .nome_arquivo = "b.txt",\
            .nome_autor = "teste",\
            .ano = 2001 \
    }; \
    Registro registro_c = { \
            .titulo = "titulo c - a volta",\
            .nome_arquivo = "c.txt",\
            .nome_autor = "testinho",\
            .ano = 2002 \
    }; \
    Registro registro_d = { \
            .titulo = "maktub", \
            .nome_arquivo = "ddd.txt", \
            .nome_autor = "paulo coelho", \
            .ano = 2003 \
    }; \
    Registro registro_e = { \
            .titulo = "Meio do Caminho", \
            .nome_arquivo = "eagora.txt", \
            .nome_autor = "Carlos Drummond", \
            .ano = 1967 \
    }

#endif //COMMON_H
