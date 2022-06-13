/*!
 * MATA54 - Estruturas de Dados e Algoritmos II
 * Terceiro Trabalho Prático
 *
 * Árvore k-d
 * Data 5 de Junho de 2022
 *
 * Aluna:
 * 	Alana Bispo de Souza
 */

// Desabilita alguns warnings gerados pela IDE. Todos eles são seguros de serem mantidos
// e no máximo geram algum tipo de warning se não forem reconhecidos pelo compilador
// fonte: https://community.rti.com/kb/why-do-i-see-warnings-about-undefined-pragmas-headers-during-compilation
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "misc-no-recursion"
#pragma ide diagnostic ignored "cert-err34-c"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "lib/utils.h"

#include "lib/registro.h"
#include "lib/arvore_2d.h"
#include "lib/pagina.h"

// Para habilitar algum debug visual descomente a linha abaixo
//#define DEBUG

// -------- Constante Especial -------- //

//             - ! AVISO ! -
// NREGSPORPAGINA, NIVEL_RAIZ entre outras constantes
// comuns ao projeto estão no seguinte arquivo
#include "lib/constantes.h"

// ------------- Comandos ------------- //
#define COMANDO_INSERIR_REGISTRO    		'i'
#define COMANDO_CONSULTAR_NOME   			'c'
#define COMANDO_CONSULTA_FAIXA_NOME      	'n'
#define COMANDO_CONSULTA_FAIXA_ANOS	        'a'
#define COMANDO_CONSULTA_FAIXA_NOME_E_ANOS  'q'
#define COMANDO_IMPRIME_INDICE_ARVORE  		't'
#define COMANDO_IMPRIME_PAGINA  			'p'
#define COMANDO_SAIR                		'e'

// ------------- Sistema ------------- //
const char *PATH_ARQUIVO_INDICES = "trabalho.indices.dat";
const char *PATH_ARQUIVO_PAGINAS = "trabalho.paginas.dat";
const char *PATH_ARQUIVO_TEMP    = "trabalho.tmp.dat";

#define DEBUG_STRING               "[DEBUG] {info} - "

// ------------- Estruturas ------------- //

/**
 * TiposBusca - Define tipo de buscas que podem ser utilizados
 */
typedef enum {
    BUSCA_NOME_AUTOR = 0,
    BUSCA_TITULO,
    BUSCA_ANO,
    BUSCA_REGISTRO
} TiposBusca;


// -------- Variáveis Globais ------- //

// TODO insert here global variables if need

// ------------- Funções Comandos ------------- //

/**
 * PROTOTIPO
 */
//void inserirRegistro(Registro *registro) {
//
//    // TODO!! MUITO ERRADO CORRIGIR
//
//    // Começamos a leitura a partir da raiz
//    NoIndice indice = leIndice(0);
//
//    // Se estivermos lendo a raiz e esta estiver vazia
//    if (indice.nivel == NIVEL_RAIZ && indice.tipo == NO_VAZIO) {
//        indice.tipo = NO_IMPAR;
//
//        // Copia o valor para o indice desse nível
//        strncpy(indice.nome_autor, registro->nome_autor, TAMANHO_MAX_STRING + 1);
//
//        // Cria as primeiras páginas
//        indice.tipo_esquerda = APONTAMENTO_PAGINA;
//        indice.tipo_direita = APONTAMENTO_PAGINA;
//        indice.esquerda = 0;
//        indice.direita = 1;
//
//        PaginaRegistros pagina0 = newPaginaRegistros();
//        PaginaRegistros pagina1 = newPaginaRegistros();
//
//        // Como esse é o primeiro nó, iremos gravar no lado esquerdo
//        pagina0.n_registros++;
//        pagina0.registros[0] = *registro;
//
//        // Le os cabecalhos
//        CabecalhoNosIndices cabecalho_indices = leCabecalhoIndices();
//        CabecalhoPaginasRegistros cabecalho_paginas = leCabecalhoPaginasRegistros();
//
//        // Incrementa os valores
//        cabecalho_indices.n_indices++;
//        cabecalho_paginas.n_registros++;
//        cabecalho_paginas.n_paginas = 2;
//
//        // Salva tudo
//        gravaIndice(0, &indice);
//        gravaPagina(0, &pagina0);
//        gravaPagina(1, &pagina1);
//        gravaCabecalhoIndices(&cabecalho_indices);
//        gravaCabecalhoPaginas(&cabecalho_paginas);
//
//        return;
//    }
//
//    // Como a raiz não está vazia iremos percorrer até encontrar uma página
//    if (comparaNivel(&indice, registro) <= 0) {
//        // Caminha pela esquerda
//        inserirRegistroRec(registro, indice.esquerda);
//        return;
//    }
//
//    // Caminha Pela direita
//    inserirRegistroRec(registro, indice.direita);
//}

/**
 * PROTOTIPO
 */
//RetornoBusca buscaRegistroPagina(Busca busca) {
//    NoIndice indice = leIndice(0);
//    Registro *registro;
//
//    // Armazena o ponteiro do registro ou reusa o parametro
//    if (busca.tipo_busca == BUSCA_REGISTRO) {
//        registro = &busca.registro;
//    }
//    else {
//        registro = malloc(TAMANHO_REGISTRO());
//        *registro = newRegistro();
//    }
//
//    // Salva as propriedades a depender do tipo
//    switch (busca.tipo_busca) {
//        case BUSCA_ANO:
//            registro->ano = busca.ano;
//            break;
//        case BUSCA_NOME_AUTOR:
//            strncpy(registro->nome_autor, busca.nome_autor, TAMANHO_MAX_STRING + 1);
//            break;
//        case BUSCA_TITULO:
//            strncpy(registro->titulo, busca.titulo, TAMANHO_MAX_STRING + 1);
//            break;
//        default:
//            break;
//    }
//}

/**
 * Consulta registros pelo nome do autor
 *
 * @param chave Chave a ser consultada.
 * @param registro_retorno Registro encontrado por uma consulta, use NULO se não desejar.
 * @return posição a qual a chave foi encontrada
 */
int consultarNome(char *nome, Registro *registro_retorno) {
    NoIndice indice = leIndice(0);

    do {

    } while(true);
}

/**
 * Consulta por faixa de nome
 *
 * @param nome_autor
 * @param nome_autor2
 */
void consultarFaixaNome(char *nome_autor, char *nome_autor2) {
    // TODO...
}

/**
 * Consulta por faixa de ano
 *
 * @param ano
 * @param ano2
 */
void consultarFaixaAno(int ano, int ano2) {
    // TODO...
}

/**
 * Consulta por faixa de nomes & anos
 *
 * @param nome_autor
 * @param nome_autor2
 * @param ano
 * @param ano2
 */
void consultarFaixaNomesAnos(char *nome_autor, char *nome_autor2, int ano, int ano2) {
    // TODO...
}

/* ----------------------------------------------------------------
 *                          Seção Principal
 * ---------------------------------------------------------------- */

int main() {
    long tamanho_arquivo = tamanhoArquivo(ARQUIVO_INDICE);

    int i;
    int pos;
    char nome_autor[TAMANHO_MAX_STRING + 1];
    char nome_autor2[TAMANHO_MAX_STRING + 1];
    int ano;
    int ano2;

    char opcao;

    Registro registro;

    // Checa se o arquivo existe
    if (tamanho_arquivo == ARQUIVO_INEXISTENTE) {
#ifdef DEBUG
        printf(DEBUG_STRING "arquivo inexistente. Criando novo!\n");
#endif

        // Cria todos os arquivos
        criarArquivoIndices();
        criarArquivoPaginas();
        criarArquivoTmp();

        // Registra os primeiros registros do arquivo
        int n;
        scanf(" %d", &n);

        // Armazena os n items
        for (i = 0; i < n; i++) {
            registro = newRegistro();

            scanf(" %[^\n]s", registro.nome_autor);
            scanf("%*c");
            scanf(" %[^\n]s", registro.titulo);
            scanf("%*c");
            scanf(" %d", &registro.ano);
            scanf(" %[^\n]s", registro.nome_arquivo);
            scanf("%*c");

            adicionarRegistroTmp(i, &registro);
            incluirIndice(i, &registro);
        }

        moveRegistrosTmpPaginas(n);
        printf("arvore k-d gerada\n");
    }

    do {
        scanf(" %c", &opcao);

        // Executa os comandos
        switch (opcao) {
            case COMANDO_INSERIR_REGISTRO:
                registro = newRegistro();

                scanf(" %[^\n]s", registro.nome_autor);
                scanf("%*c");
                scanf(" %[^\n]s", registro.titulo);
                scanf("%*c");
                scanf(" %d", &registro.ano);
                scanf(" %[^\n]s", registro.nome_arquivo);
                scanf("%*c");

                salvaRegistroPagina(&registro);
                break;

            case COMANDO_CONSULTAR_NOME:
                scanf(" %[^\n]s", nome_autor);
                scanf("%*c");

                if (consultarNome(nome_autor, &registro) == CHAVE_INEXISTENTE) {
                    printf("nome nao encontrado: %s\n", nome_autor);
                } else {
                    printf("nome: ");
                    imprimeRegistro(&registro);
                }

                break;

            case COMANDO_CONSULTA_FAIXA_NOME:
                scanf(" %[^\n]s", nome_autor);
                scanf("%*c");
                scanf(" %[^\n]s", nome_autor2);
                scanf("%*c");

                consultarFaixaNome(nome_autor, nome_autor2);
                break;

            case COMANDO_CONSULTA_FAIXA_ANOS:
                scanf(" %d", &ano);
                scanf(" %d", &ano2);

                consultarFaixaAno(ano, ano2);
                break;

            case COMANDO_CONSULTA_FAIXA_NOME_E_ANOS:
                scanf(" %[^\n]s", nome_autor);
                scanf("%*c");
                scanf(" %[^\n]s", nome_autor2);
                scanf("%*c");

                scanf(" %d", &ano);
                scanf(" %d", &ano2);

                consultarFaixaNomesAnos(nome_autor, nome_autor2, ano, ano2);
                break;

            case COMANDO_IMPRIME_INDICE_ARVORE:
                imprimeIndices(0);
                break;

            case COMANDO_IMPRIME_PAGINA:
                scanf(" %d", &pos);

                imprimePagina(pos);
                break;

            case COMANDO_SAIR:
            default:
                break;
        }

    } while (opcao != COMANDO_SAIR);

    return EXIT_SUCCESS;
}