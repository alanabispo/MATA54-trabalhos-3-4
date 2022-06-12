#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "misc-no-recursion"
#pragma ide diagnostic ignored "cert-err34-c"

/*!
 * MATA54 - Estruturas de Dados e Algoritmos II
 * Terceiro Trabalho Prático
 *
 * Árvore k-d
 * Data 5 de Junho de 2022
 *
 * Aluna:
 * 	Alana Bispo de Souza
 *
 * ------ CRIAR REPOSITORIO E RELATORIO ------
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Para habilitar algum debug visual descomente a linha abaixo
//#define DEBUG

// -------- Constante Especial -------- //
#define NREGSPORPAGINA                  3
#define NIVEL_RAIZ                      1

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

#define TAMANHO_MAX_STRING          (20)

#define CHAVE_INEXISTENTE           (-1)

#define ARQUIVO_INEXISTENTE         (-1)
#define TAMANHO_INVALIDO_ARQUIVO    (-2)

#define APONTADOR_VAZIO             (-1)

#define ANO_VAZIO                   (-1)
#define NIVEL_VAZIO                 (-1)

#define POSICAO_INVALIDA            (-1)

#define DEBUG_STRING                "[DEBUG] {info} - "

// ------------- Estruturas ------------- //

/**
 * Registro - Estrutura que armazena um registro da nossa estrutura
 */
typedef struct {
    char nome_autor[TAMANHO_MAX_STRING + 1]; // Adiciona +1 caractere para o \0
    char titulo[TAMANHO_MAX_STRING + 1];
    int ano;
    char nome_arquivo[TAMANHO_MAX_STRING + 1];
} Registro;

/**
 * PaginaRegistros - Armazena os registros nos nós folhas da arvore k-d
 */
typedef struct {
    int n_registros;
    int prox_pagina;
    Registro registros[NREGSPORPAGINA];
} PaginaRegistros;

/**
 * CabeçalhoListaRegistros - Metadados para armazenamento dos registros numa tabela temporária
 */
typedef struct {
    int n_registros;
    int n_paginas;
} CabecalhoPaginasRegistros;

/**
 * Define os tipos que um determinado nó de indice pode ter:
 *  - IMPAR = nome_autor
 *  - PAR = ano
 */
typedef enum {
    NO_IMPAR = 0,
    NO_PAR,
    NO_VAZIO
} TiposNosIndice;

/**
 * Tipo de apontamento que um índice pode utilizar
 */
typedef enum {
    APONTAMENTO_INDICE = 0,
    APONTAMENTO_PAGINA,
    APONTAMENTO_VAZIO
} TipoApontamento;

/**
 * NoIndice - Nó que representa um indice da página
 */
typedef struct {
    // Armazena a propriedade principal em relação ao tipo do nó
    union {
        char nome_autor[TAMANHO_MAX_STRING + 1];
        int ano;
    };

    // Tipo do nó
    TiposNosIndice tipo;

    // Nível do nó
    int nivel;

    // Apontadores para os nós filhos do nó atual
    int direita;
    int esquerda;

    // Tipos dos apontadores
    TipoApontamento tipo_direita;
    TipoApontamento tipo_esquerda;
} NoIndice;

/**
 * CabecalhoNosIndices - Cabeçalho relacionado aos indices dos nós
 */
typedef struct {
    int n_indices;
} CabecalhoNosIndices;

/**
 * TiposArquivo - Tipos de arquivo que podem ser utilizados
 */
typedef enum {
    ARQUIVO_INDICE = 0,
    ARQUIVO_PAGINAS
} TiposArquivo;

/**
 * TiposBusca - Define tipo de buscas que podem ser utilizados
 */
typedef enum {
    BUSCA_NOME_AUTOR = 0,
    BUSCA_TITULO,
    BUSCA_ANO,
    BUSCA_REGISTRO
} TiposBusca;

/**
 * Busca - Dados para uma busca
 */
typedef struct {
    TiposBusca tipo_busca;
    union {
        char nome_autor[TAMANHO_MAX_STRING + 1];
        char titulo[TAMANHO_MAX_STRING + 1];
        int ano;
        Registro registro;
    };
} Busca;

/**
 * RetornoBusca - Dados de retorno a uma busca
 */
typedef struct {
    int pagina;
    int item;
} RetornoBuscaPosicaoSalvar;

// -------- Variáveis Globais ------- //

// TODO insert here global variables if need

// ------------- Macros ------------- //
#define ABRIR_ARQUIVO_INDICES_LEITURA()               fopen(PATH_ARQUIVO_INDICES, "rb")
#define ABRIR_ARQUIVO_INDICES_ESCRITA()               fopen(PATH_ARQUIVO_INDICES, "wb")
#define ABRIR_ARQUIVO_INDICES_LEITURA_ESCRITA()       fopen(PATH_ARQUIVO_INDICES, "rb+")

#define ABRIR_ARQUIVO_PAGINAS_LEITURA()               fopen(PATH_ARQUIVO_PAGINAS, "rb")
#define ABRIR_ARQUIVO_PAGINAS_ESCRITA()               fopen(PATH_ARQUIVO_PAGINAS, "wb")
#define ABRIR_ARQUIVO_PAGINAS_LEITURA_ESCRITA()       fopen(PATH_ARQUIVO_PAGINAS, "rb+")

#define ABRIR_ARQUIVO_TMP_LEITURA()                   fopen(PATH_ARQUIVO_TEMP, "rb")
#define ABRIR_ARQUIVO_TMP_ESCRITA()                   fopen(PATH_ARQUIVO_TEMP, "wb")
#define ABRIR_ARQUIVO_TMP_LEITURA_ESCRITA()           fopen(PATH_ARQUIVO_TEMP, "rb+")

#define TAMANHO_REGISTRO()                            sizeof(Registro)
#define TAMANHO_INDICE()                              sizeof(NoIndice)
#define TAMANHO_PAGINA()                              sizeof(PaginaRegistros)
#define TAMANHO_CABECALHO_INDICE()                    sizeof(CabecalhoNosIndices)
#define TAMANHO_CABECALHO_PAGINAS()                   sizeof(CabecalhoPaginasRegistros)

#define max(A, B)                                     ((A) > (B) ? (A) : (B))

// ------------- Funções Estruturas ------------- //

/**
 * Cria um registro vazio
 *
 * @return Registro vazio.
 */
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

/**
 * Compara se dois registros são iguais
 *
 * @param a Registro A
 * @param b Registro B
 * @return Retorna o resultado da comparação dos dois registros
 */
bool equalsRegistro(Registro *a, Registro *b) {
    return (
        a->ano == b->ano &&
        strncmp(a->nome_autor, b->nome_autor, TAMANHO_MAX_STRING + 1) == 0 &&
        strncmp(a->titulo, b->titulo, TAMANHO_MAX_STRING + 1) == 0 &&
        strncmp(a->nome_arquivo, b->nome_arquivo, TAMANHO_MAX_STRING + 1) == 0
    );
}

/**
 * Cria uma página de registros
 *
 * @return Página de registros vazia
 */
PaginaRegistros newPaginaRegistros() {
    int i;
    PaginaRegistros pagina;

    // Zera propriedades
    pagina.n_registros = 0;
    pagina.prox_pagina = APONTADOR_VAZIO;

    // Zera os items da página
    for (i = 0; i < NREGSPORPAGINA; i++) {
        pagina.registros[i] = newRegistro();
    }

    return pagina;
}

/**
 * Cria um nó indice vazio
 *
 * @return Nó indice vazio.
 */
NoIndice newNoIndice() {
    int i;
    NoIndice noIndice;

    // Inicializa os apontadores do índice
    noIndice.esquerda = APONTADOR_VAZIO;
    noIndice.direita = APONTADOR_VAZIO;

    // Inicializa outras propriedades
    noIndice.ano = ANO_VAZIO;
    noIndice.nivel = NIVEL_VAZIO;
    noIndice.tipo = NO_VAZIO;
    noIndice.tipo_direita = APONTAMENTO_VAZIO;
    noIndice.tipo_esquerda = APONTAMENTO_VAZIO;

    // Zera vetores
    for (i = 0; i < (TAMANHO_MAX_STRING + 1); i++) {
        noIndice.nome_autor[i] = '\0';
    }

    return noIndice;
}

/**
 * Cria um novo cabecalho da página de registros vazio
 *
 * @return Cabecalho pagina de registros vazios
 */
CabecalhoPaginasRegistros newCabecalhoPaginasRegistros() {
    CabecalhoPaginasRegistros cabecalho;

    cabecalho.n_registros = 0;
    cabecalho.n_paginas = 0;

    return cabecalho;
}

/**
 * Cria um novo cabecalho de nos indices vazio
 *
 * @return Cabecalho dos indices vazio.
 */
CabecalhoNosIndices newCabecalhoNosIndices() {
    CabecalhoNosIndices cabecalho;

    cabecalho.n_indices = 0;

    return cabecalho;
}

// ------------- Funções Utilitárias ------------- //

/**
 * Imprime um registro.
 *
 * @param registro Dados do registro a ser impresso.
 */
void imprimeRegistro(Registro *registro) {
    printf("%s\n%s\n%d\n%s\n", registro->nome_autor, registro->titulo, registro->ano, registro->nome_arquivo);
}

/**
 * Remove caracteres estranhos depois do primeiro caractere \0
 * @param str
 */
void removeCaracteresEstranhosString(char *str) {
    size_t i;
    size_t tamanho_string = strnlen(str, TAMANHO_MAX_STRING + 1);

    for (i = tamanho_string; i < (TAMANHO_MAX_STRING + 1); i++) {
        str[i] = '\0';
    }
}

/**
 * Compara inteiro para ordenação crescente
 *
 * @param a Valor a
 * @param b Valor b
 * @return Retorna 1 quando (a > b), 0 quando (a == b) e -1 quando (a < b)
 */
int cmp_inteiro(const void *a_ptr, const void *b_ptr) {
    int a = *(const int *)a_ptr;
    int b = *(const int *)b_ptr;

    if (a > b) {
        return 1;
    }

    if (b < a) {
        return -1;
    }

    return 0;
}

/**
 * Le cabecalho dos indices
 *
 * @return cabecalho dos indices
 */
CabecalhoNosIndices leCabecalhoIndices() {
    FILE *arquivo = ABRIR_ARQUIVO_INDICES_LEITURA();
    CabecalhoNosIndices cabecalho = newCabecalhoNosIndices();

    fread(&cabecalho, TAMANHO_CABECALHO_INDICE(), 1, arquivo);
    fclose(arquivo);

    return cabecalho;
}

/**
 * Le cabecalho das páginas
 *
 * @return cabecalho das pagina
 */
CabecalhoPaginasRegistros leCabecalhoPaginasRegistros() {
    FILE *arquivo = ABRIR_ARQUIVO_PAGINAS_LEITURA();
    CabecalhoPaginasRegistros cabecalho = newCabecalhoPaginasRegistros();

    fread(&cabecalho, TAMANHO_CABECALHO_PAGINAS(), 1, arquivo);
    fclose(arquivo);

    return cabecalho;
}

/**
 * Le indice do arquivo de indices
 *
 * @param pos Posição do arquivo a ser lida
 * @return Indice lido a partir do arquivo
 */
NoIndice leIndice(int pos) {
    FILE *arquivo = ABRIR_ARQUIVO_INDICES_LEITURA();

    // Indice que será  retornando
    NoIndice indice = newNoIndice();

    // Caminha até o indice desejado
    fseek(arquivo, TAMANHO_CABECALHO_INDICE(), SEEK_CUR);
    fseek(arquivo, TAMANHO_INDICE() * pos, SEEK_CUR);

    // Lê um indice
    fread(&indice, TAMANHO_INDICE(), 1, arquivo);
    fclose(arquivo);

    return indice;
}

/**
 * Le pagina de registros do arquivo de páginas
 *
 * @param pos posição do arquivo a ser lida
 * @return Página de registros obtida do arquivo
 */
PaginaRegistros lePagina(int pos) {
    FILE *arquivo = ABRIR_ARQUIVO_PAGINAS_LEITURA();

    // Página que será retornada
    PaginaRegistros pagina = newPaginaRegistros();

    // Caminha até a página desejada
    fseek(arquivo, TAMANHO_CABECALHO_PAGINAS(), SEEK_CUR);
    fseek(arquivo, TAMANHO_PAGINA() * pos, SEEK_CUR);

    // Lê uma página
    fread(&pagina, TAMANHO_PAGINA(), 1, arquivo);
    fclose(arquivo);

    return pagina;
}

/**
 * Grava uma página de registros na memória
 *
 * @param pos
 * @param pagina
 */
void gravaPagina(int pos, PaginaRegistros *pagina) {
    FILE *arquivo = ABRIR_ARQUIVO_PAGINAS_LEITURA_ESCRITA();

    // Caminha até o local desejado
    fseek(arquivo, TAMANHO_CABECALHO_PAGINAS(), SEEK_CUR);
    fseek(arquivo, TAMANHO_PAGINA() * pos, SEEK_CUR);

    // Grava uma página
    fwrite(pagina, TAMANHO_PAGINA(), 1, arquivo);
    fclose(arquivo);
}

/**
 * Grava um indice no arquivo que representa a arvore
 *
 * @param pos Posição a ser gravada no arquivo
 * @param indice Dados do indice
 */
void gravaIndice(int pos, NoIndice *indice) {
    FILE *arquivo = ABRIR_ARQUIVO_INDICES_LEITURA_ESCRITA();

    // Caminha até o local desejado
    fseek(arquivo, TAMANHO_CABECALHO_INDICE(), SEEK_CUR);
    fseek(arquivo, TAMANHO_INDICE() * pos, SEEK_CUR);

    // Grava um indice
    fwrite(indice, TAMANHO_INDICE(), 1, arquivo);
    fclose(arquivo);
}

/**
 * Grava cabecalho dos nos indices
 *
 * @param cabecalho Cabecalho a ser gravado
 */
void gravaCabecalhoIndices(CabecalhoNosIndices *cabecalho) {
    FILE *arquivo = ABRIR_ARQUIVO_INDICES_LEITURA_ESCRITA();

    // Grava cabecalho dos indices
    fwrite(cabecalho, TAMANHO_CABECALHO_INDICE(), 1, arquivo);
    fclose(arquivo);
}

/**
 * Grava cabecalho paginas
 *
 * @param cabecalho Cabecalho a ser gravado
 */
void gravaCabecalhoPaginas(CabecalhoPaginasRegistros *cabecalho) {
    FILE *arquivo = ABRIR_ARQUIVO_PAGINAS_LEITURA_ESCRITA();

    // Grava cabecalho dos registros
    fwrite(cabecalho, TAMANHO_CABECALHO_PAGINAS(), 1, arquivo);
    fclose(arquivo);
}

/**
 * Verifica o tamanho do arquivo binário, e retorna o mesmo.
 *
 * @param tipo_arquivo Qual arquivo a ser lido. 0 = arquivo principal, 1 = arquivo temporário
 * @return Retorna o tamanho do arquivo de registros, ou diz se ele é válido
 */
long tamanhoArquivo(TiposArquivo tipo_arquivo) {
    FILE* arquivo = tipo_arquivo == ARQUIVO_INDICE
                    ? ABRIR_ARQUIVO_INDICES_LEITURA()
                    : ABRIR_ARQUIVO_PAGINAS_LEITURA();
    long tamanho;

    // Falha ao abrir arquivo
    if (arquivo == NULL) {
        return ARQUIVO_INEXISTENTE;
    }

    // Percorre o arquivo até o final
    if (fseek(arquivo, 0, SEEK_END)) {
        fclose(arquivo);
        return TAMANHO_INVALIDO_ARQUIVO;
    }

    // Informa onde o ponteiro de leitura parou (esse é o tamanho do arquivo)
    tamanho = ftell(arquivo);
    fclose(arquivo);

    return tamanho;
}

/**
 * Cria arquivo de indices
 */
void criarArquivoIndices() {
    FILE *arquivo = ABRIR_ARQUIVO_INDICES_ESCRITA();

    // Cria indice raiz e armazena no arquivo
    NoIndice no_raiz = newNoIndice();
    no_raiz.nivel = NIVEL_RAIZ;

    // Grava raiz no arquivo
    fwrite(&no_raiz, TAMANHO_INDICE(), 1, arquivo);
    fclose(arquivo);
}

/**
 * Cria arquivo Páginas
 */
void criarArquivoPaginas() {
    FILE *arquivo = ABRIR_ARQUIVO_PAGINAS_ESCRITA();
    fclose(arquivo);
}

/**
 * Cria arquivo temporário
 */
void criarArquivoTmp() {
    FILE *arquivo = ABRIR_ARQUIVO_TMP_ESCRITA();
    fclose(arquivo);
}

// ------------- Funções Comandos ------------- //

/**
 * Compara o registro com o indice atual com base no nível
 *
 * @param indice Indice atual
 * @param registro Registro a ser comparado
 * @return < 0 registro menor que indice, 0 se igual e > 0 se maior que indice
 */
int comparaNivel(NoIndice *indice, Registro *registro) {
    // Compara Nome autor no nível impar
    if (indice->nivel % 2 == 1) {
        return strncmp(registro->nome_autor, indice->nome_autor, TAMANHO_MAX_STRING + 1);
    }

    // Compara ano no nível par
    return registro->ano - indice->ano;
}

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
 * Adiciona registro temporariamente
 *
 * @param registro
 */
void adicionarRegistroTmp(int pos, Registro *registro) {
    FILE *arquivo = ABRIR_ARQUIVO_TMP_LEITURA_ESCRITA();

    // Avança até a posição e grava
    fseek(arquivo, TAMANHO_REGISTRO() * pos, SEEK_CUR);
    fwrite(registro, TAMANHO_REGISTRO(), 1, arquivo);

    fclose(arquivo);
}

/**
 * Retorna um registro de um arquivo temporário
 *
 * @param pos Posição a ser retornada
 * @return Registro lido a partir do arquivo temporário
 */
Registro leRegistroTmp(int pos) {
    FILE *arquivo = ABRIR_ARQUIVO_TMP_LEITURA();
    Registro registro = newRegistro();

    // Avança até a posição e grava
    fseek(arquivo, TAMANHO_REGISTRO() * pos, SEEK_CUR);
    fread(&registro, TAMANHO_REGISTRO(), 1, arquivo);

    fclose(arquivo);

    return registro;
}

/**
 * Copia a propriedade nome autor para o indice
 *
 * @param indice Indice a ser copiado propriedade nome_autor
 * @param registro Registro de origem da propriedade nome_autor
 */
void copiaNomeAutorIndice(NoIndice *indice, Registro *registro) {
    strncpy(indice->nome_autor, registro->nome_autor, TAMANHO_MAX_STRING + 1);
}

/**
 * Função auxiliar para lidar com a recursividade da operação
 *
 * @param idx Index do item que vai ser adicionado
 * @param pos Posição da arvore sendo lida
 * @param nivel nivel do indice na arvore
 * @param registro Registro que está construindo o item
 */
void incluirIndiceRec(int idx, int pos, Registro *registro) {
    // Começamos a leitura a partir da raiz
    NoIndice indice = leIndice(pos);

    // Caminha para esquerda
    if (comparaNivel(&indice, registro) <= 0) {
        // Se a posição ainda não estiver sendo utilizada registra ela
        if (indice.tipo_esquerda == APONTAMENTO_VAZIO) {
            indice.tipo_esquerda = APONTAMENTO_INDICE;
            indice.esquerda = idx;

            // Grava indice pai
            gravaIndice(pos, &indice);

            // Grava novo indice vazio na nova posição
            NoIndice indice_filho = newNoIndice();
            indice_filho.nivel++;
            indice_filho.ano = registro->ano;
            indice_filho.tipo = NO_PAR;

            gravaIndice(idx, &indice_filho);

            return;
        }

        // Caminha pela esquerda
        incluirIndiceRec(idx, indice.esquerda, registro);
        return;
    }

    // Se a posição ainda não estiver sendo utilizada registra ela
    if (indice.tipo_direita == APONTAMENTO_VAZIO) {
        indice.tipo_direita = APONTAMENTO_INDICE;
        indice.direita = idx;

        // Grava indice pai
        gravaIndice(pos, &indice);

        // Grava novo indice vazio na nova posição
        NoIndice indice_filho = newNoIndice();
        indice_filho.nivel++;
        indice_filho.ano = registro->ano;
        indice_filho.tipo = NO_PAR;

        gravaIndice(idx, &indice_filho);

        return;
    }

    // Caminha Pela direita
    incluirIndiceRec(idx, indice.direita, registro);
}

/**
 * Inclui um novo indice a estrutura de indices, sem salvar a página
 *
 * @param idx Index do indice a ser adicionado
 * @param registro Registro a ser adicionado
 */
void incluirIndice(int idx, Registro *registro) {
    // Começamos a leitura a partir da raiz
    NoIndice indice = leIndice(0);

    // Incrementa o número de índices no cabeçalho
    CabecalhoNosIndices cabecalho = leCabecalhoIndices();
    cabecalho.n_indices++;
    gravaCabecalhoIndices(&cabecalho);

    // O primeiro index é a raiz
    if (idx == 0) {
        indice.tipo = NO_IMPAR;
        indice.nivel = NIVEL_RAIZ;

        // Copia o nome_autor para o indice
        copiaNomeAutorIndice(&indice, registro);

        // Grava o indice
        gravaIndice(0, &indice);

        return;
    }

    // Como não é a primeira posição, iremos seguir com as inserções recursivas
    if (comparaNivel(&indice, registro) <= 0) {
        // Se a posição ainda não estiver sendo utilizada registra ela
        if (indice.tipo_esquerda == APONTAMENTO_VAZIO) {
            indice.tipo_esquerda = APONTAMENTO_INDICE;
            indice.esquerda = idx;

            // Grava indice pai
            gravaIndice(0, &indice);

            // Grava novo indice vazio na nova posição
            NoIndice indice_filho = newNoIndice();
            indice_filho.nivel++;
            indice_filho.ano = registro->ano;
            indice_filho.tipo = NO_PAR;

            gravaIndice(1, &indice_filho);

            return;
        }

        // Caminha pela esquerda
        incluirIndiceRec(idx, indice.esquerda, registro);
        return;
    }

    // Se a posição ainda não estiver sendo utilizada registra ela
    if (indice.tipo_direita == APONTAMENTO_VAZIO) {
        indice.tipo_direita = APONTAMENTO_INDICE;
        indice.direita = idx;

        // Grava indice pai
        gravaIndice(0, &indice);

        // Grava novo indice vazio na nova posição
        NoIndice indice_filho = newNoIndice();
        indice_filho.nivel++;
        indice_filho.ano = registro->ano;
        indice_filho.tipo = NO_PAR;

        gravaIndice(1, &indice_filho);

        return;
    }

    // Caminha Pela direita
    incluirIndiceRec(idx, indice.direita, registro);
}

/**
 * Cria páginas de maneira recursiva em abordagem pre ordem
 *
 * @param pos Posição que está sendo lida
 * @param n_paginas Numero de páginas já registrado
 */
void criarPaginasRec(int pos, int *n_paginas) {
    NoIndice indice = leIndice(pos);

    // Acessa o nó a esquerda ou cria página
    if (indice.tipo_esquerda == APONTAMENTO_INDICE) {
        criarPaginasRec(indice.esquerda, n_paginas);
    } else if (indice.tipo_esquerda == APONTAMENTO_VAZIO) {
        indice.tipo_esquerda = APONTAMENTO_PAGINA;
        indice.esquerda = *n_paginas;

        // Atualiza o número de páginas
        CabecalhoPaginasRegistros cabecalho = leCabecalhoPaginasRegistros();
        cabecalho.n_paginas++;
        gravaCabecalhoPaginas(&cabecalho);

        // Cria a página
        PaginaRegistros pagina = newPaginaRegistros();
        gravaPagina(*n_paginas, &pagina);

        // Incrementa o número de páginas
        *n_paginas = *n_paginas + 1;
    }

    // Acessa o nó a direita ou cria página
    if (indice.tipo_direita == APONTAMENTO_INDICE) {
        criarPaginasRec(indice.direita, n_paginas);
    } else if (indice.tipo_direita == APONTAMENTO_VAZIO) {
        indice.tipo_direita = APONTAMENTO_PAGINA;
        indice.direita = *n_paginas;

        // Atualiza o número de páginas
        CabecalhoPaginasRegistros cabecalho = leCabecalhoPaginasRegistros();
        cabecalho.n_paginas++;
        gravaCabecalhoPaginas(&cabecalho);

        // Cria a página
        PaginaRegistros pagina = newPaginaRegistros();
        gravaPagina(*n_paginas, &pagina);

        // Incrementa o número de páginas
        *n_paginas = *n_paginas + 1;
    }
}

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
 * Encontra o melhor local para salvar um registro ou retorna a posição que encontrou o
 * mesmo registro dentro de numa página
 *
 * @param registro Registro a ser buscado
 * @return Pagina e indice para salvar um documento
 */
RetornoBuscaPosicaoSalvar salvaRegistroPagina(Registro *registro) {
    int pos_pagina = POSICAO_INVALIDA;
    int pos_indice = 0;
    NoIndice indice;

    // Encontra a página adequada
    do {
        indice = leIndice(pos_indice);

        // Caminha pela esquerda
        if (comparaNivel(&indice, registro) <= 0) {
            if (indice.tipo_esquerda == APONTAMENTO_INDICE) {
                pos_indice = indice.esquerda;
            }
            else if (indice.tipo_esquerda == APONTAMENTO_PAGINA) {
                pos_pagina = indice.esquerda;
                break;
            }
            else {
                pos_indice = POSICAO_INVALIDA;
            }
        }
        else {
            if (indice.tipo_direita == APONTAMENTO_INDICE) {
                pos_indice = indice.direita;
            }
            else if (indice.tipo_direita == APONTAMENTO_PAGINA) {
                pos_pagina = indice.direita;
                break;
            }
            else {
                pos_indice = POSICAO_INVALIDA;
            }
        }
    } while (indice.tipo != NO_VAZIO && pos_indice != POSICAO_INVALIDA);

    // Problema encontrado
    if (indice.tipo == NO_VAZIO || pos_indice == POSICAO_INVALIDA || pos_pagina == POSICAO_INVALIDA) {
        return (RetornoBuscaPosicaoSalvar){ .pagina = POSICAO_INVALIDA, .item = POSICAO_INVALIDA };
    }

    // Procura o registro na página ou nas páginas aninhadas
    //      SE Registro encontrado → retorna
    //      SENÃO Armazena
    //          SE Página estiver cheia, cria uma página antes
    //          Armazena o registro
    int i;
    int pos_pagina_atual = pos_pagina;
    PaginaRegistros pagina;
    do {
        pos_pagina = pos_pagina_atual;
        pagina = lePagina(pos_pagina_atual);

        // Procura o item na página atual
        for (i = 0; i < pagina.n_registros; i++) {
            if (equalsRegistro(&pagina.registros[i], registro)) {
                return (RetornoBuscaPosicaoSalvar){.pagina = pos_pagina, .item = i };
            }
        }

        // Se a página estiver cheia e não tivermos encontrado caminha para próxima
        pos_pagina_atual = pagina.prox_pagina;
    } while (pagina.n_registros == NREGSPORPAGINA && pos_pagina_atual != APONTADOR_VAZIO);

    CabecalhoPaginasRegistros cabecalho = leCabecalhoPaginasRegistros();
    cabecalho.n_registros++;

    // Se a página não tem posições livres
    if (pagina.n_registros == NREGSPORPAGINA) {
        // Atualiza a página atual
        int pos_nova_pagina = cabecalho.n_paginas;
        pagina.prox_pagina = pos_nova_pagina;
        cabecalho.n_paginas++;
        gravaPagina(pos_pagina, &pagina);

        // Cria nova página
        pos_pagina = pos_nova_pagina;
        pagina = newPaginaRegistros();
    }

    // Insere o novo registro
    int pos_item = pagina.n_registros;
    pagina.n_registros++;
    cabecalho.n_registros++;
    pagina.registros[pos_item] = *registro;

    gravaCabecalhoPaginas(&cabecalho);
    gravaPagina(pos_pagina, &pagina);

    return (RetornoBuscaPosicaoSalvar){.pagina = pos_pagina, .item = pos_item };
}

/**
 * Salva todas as páginas com os registros na lista de registros temporários
 *
 * @param n número de registros
 */
void salvarPaginas(int n) {
    int i;
    int n_paginas = 0;
    Registro registro;

    // Percorre o indice em pŕe ordem e registra apontadores para as páginas
    criarPaginasRec(0, &n_paginas);

    // Salva os demais registros
    for (i = 0; i < n; i++) {
        registro = leRegistroTmp(i);
        salvaRegistroPagina(&registro);
    }
}

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

/**
 * Imprime todos os indices em ordem
 */
void imprimeIndices(int pos) {
    NoIndice esquerda;
    NoIndice direita;
    NoIndice indice = leIndice(pos);

    // Não faz nada
    if (indice.tipo == NO_VAZIO) {
        return;
    }

    // Percorre a esquerda
    if (indice.tipo_esquerda == APONTAMENTO_INDICE) {
        imprimeIndices(indice.esquerda);
    }

    // Verifica qual o lado
    if (indice.tipo == NO_IMPAR) {
        printf("autor: %s ", indice.nome_autor);

        // Imprime o nó a esquerda
        if (indice.tipo_esquerda == APONTAMENTO_PAGINA) {
            printf("fesq: pagina ");
        }
        else {
            esquerda = leIndice(indice.esquerda);
            printf("fesq: %d ", esquerda.ano);
        }

        // Imprime o nó a direita
        if (indice.tipo_direita == APONTAMENTO_PAGINA) {
            printf("fdir: pagina ");
        }
        else {
            direita = leIndice(indice.direita);
            printf("fdir: %d\n", direita.ano);
        }
    }
    else {
        printf("ano: %d ", indice.ano);

        // Imprime o nó a esquerda
        if (indice.tipo_esquerda == APONTAMENTO_PAGINA) {
            printf("fesq: pagina ");
        }
        else {
            esquerda = leIndice(indice.esquerda);
            printf("fesq: %s ", esquerda.nome_autor);
        }

        // Imprime o nó a direita
        if (indice.tipo_direita == APONTAMENTO_PAGINA) {
            printf("fdir: pagina ");
        }
        else {
            direita = leIndice(indice.direita);
            printf("fdir: %s\n", direita.nome_autor);
        }
    }

    // Percorre a direita
    if (indice.tipo_direita == APONTAMENTO_INDICE) {
        imprimeIndices(indice.direita);
    }
}

/**
 * Imprime a página com o índice especificado
 *
 * @param pos
 */
void imprimePagina(int pos) {
    int i;
    PaginaRegistros pagina;

    printf("pagina: %d\n", pos);
    do {
        pagina = lePagina(pos);

        // Imprime os registros da página
        for (i = 0; i < pagina.n_registros; i++) {
            imprimeRegistro(&pagina.registros[i]);
        }

        pos = pagina.prox_pagina;
    } while(pagina.n_registros == NREGSPORPAGINA && pos != POSICAO_INVALIDA);
}


/* ----------------------------------------------------------------
 *                          Seção Principal
 * ---------------------------------------------------------------- */

int main(__attribute__((unused)) int argc, __attribute__((unused))  char *argv[]) {
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

        // Cria ambos os arquivos
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

        salvarPaginas(n);
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