/*!
 * MATA54 - Estruturas de Dados e Algoritmos II
 * Segundo Trabalho Prático
 *
 * Hashing Perfeito
 * Data 1 de Maio de 2022
 *
 * Alunas:
 * 	Alana Bispo de Souza
 * 
 * ------ CRIAR REPOSITORIO E RELATORIO ------
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

// Para habilitar o debug descomente a linha abaixo
//#define DEBUG

// -------- Constante Especial -------- //
#define MAXNUMREGS 100
#define A           10
#define B           10

// ------------- Comandos ------------- //
#define COMANDO_INSERIR_REGISTRO    'i'
#define COMANDO_CONSULTAR_REGISTRO  'c'
#define COMANDO_IMPRIMIR_NIVEL_1    'p'
#define COMANDO_IMPRIMIR_NIVEL_2    's'
#define COMANDO_ESTRUTURA_GLOBAL    'h'
#define COMANDO_CARDINALIDADE       'n'
#define COMANDO_SAIR                'e'

// ------------- Sistema ------------- //
const char* ARQUIVO_SAIDA = "trabalho.dat";
#define TAMANHO_MAX_STRING          20

#define CHAVE_VAZIA                 (-1)
#define CHAVE_REMOVIDA              (-2)

#define CHAVE_INEXISTENTE           (-1)
#define CHAVE_EXISTENTE             (-2)

#define ARQUIVO_CHEIO				(-1)

#define ARQUIVO_INEXISTENTE         (-1)
#define TAMANHO_INVALIDO_ARQUIVO    (-2)

#define DEBUG_STRING                "[DEBUG] {info} - "

// ------------- Estruturas ------------- //
typedef struct {
    unsigned int chave;
    unsigned int idade;
    char nome[TAMANHO_MAX_STRING + 1]; // Adiciona +1 caractere para o \0
} Registro;

typedef struct {
    double a;
    double b;
} TuplaDouble;

// ------------- Macros ------------- //
#define ABRIR_ARQUIVO_LEITURA()         fopen(ARQUIVO_SAIDA, "rb")
#define ABRIR_ARQUIVO_ESCRITA()         fopen(ARQUIVO_SAIDA, "wb")
#define ABRIR_ARQUIVO_LEITURA_ESCRITA() fopen(ARQUIVO_SAIDA, "rb+")
#define TAMANHO_REGISTRO()              sizeof(Registro)

#define max(A, B)                       ((A) > (B) ? (A) : (B))

// ------------- Funções Hash ------------- //

/**
 * A função de h1, recebe uma chave e retorna o resto dessa chave pelo tamanho do arquivo.
 *
 * @param chave Chave que irá passar pelo processo de hash.
 * @return Endereço de retorno após execução do hash.
 */
int h1(unsigned int chave){
    int hash_saida = (int)(chave % MAXNUMREGS);

    return hash_saida;
}

/**
 * Função de h2, recebe uma chave e retorna o maximo entre o chao da chave divido pelo tamanho do arquivo e 1.
 *
 * @param chave Chave a ser executado o hash.
 * @return Endereço desejado após a execução da função de hash.
 */
int h2(unsigned int chave){
    double hash_saida = max(floor((double)chave / MAXNUMREGS), 1);

    return (int) hash_saida;
}

/**
 * Função de Hash principal h, que recebe uma chave e um iterador, e retorna a posição onde a chave está armazenada.
 *
 * @param i Iterador
 * @param chave Chave a ser consultada.
 * @return Endereço do registro no arquivo de registros.
 */
int h(int i, unsigned int chave){
    int hash_saida = ((h1(chave) + (i * h2(chave))) % MAXNUMREGS);

    return hash_saida;
}

// ------------- Funções Utilitárias ------------- //

/**
 * Imprime um registro.
 *
 * @param registro Dados do registro a ser impresso.
 */
void imprimirRegistro(Registro registro){
    printf("%d\n%s\n%d\n", registro.chave, registro.nome, registro.idade);
}

/**
 * Cria e inicializa o arquivo de registros.
 */
void criaArquivo(){
    FILE* arquivo = ABRIR_ARQUIVO_ESCRITA();
    int i;

    // Verifica se foi possível abrir o arquivo
    if(arquivo == NULL){
        exit(EXIT_FAILURE);
    }

    // Cria registro vazios
    Registro registro_vazio;
    registro_vazio.chave = CHAVE_VAZIA;
    registro_vazio.nome[0] = '\0';
    registro_vazio.idade = 0;

    // Grava o registro vazio em todas as posições do arquivo
    for(i = 0; i < MAXNUMREGS; i++){
        fwrite(&registro_vazio, TAMANHO_REGISTRO(), 1, arquivo);
    }

    // Fecha o arquivo
    fclose(arquivo);
}

/**
 * Verifica o tamanho do arquivo binário, e retorna o mesmo.
 *
 * @return Retorna o tamanho do arquivo de registros, ou diz se ele é válido
 */
long tamanhoArquivo() {
    FILE* arquivo = ABRIR_ARQUIVO_LEITURA();
    long tamanho;

    // Falha ao abrir arquivo
    if(arquivo == NULL){
        return ARQUIVO_INEXISTENTE;
    }

    // Percorre o arquivo até o final
    if(fseek(arquivo, 0, SEEK_END)){
        fclose(arquivo);
        return TAMANHO_INVALIDO_ARQUIVO;
    }

    // Informa onde o ponteiro de leitura parou (esse é o tamanho do arquivo)
    tamanho = ftell(arquivo);
    fclose(arquivo);

    return tamanho;
}

// ------------- Funções Comandos ------------- //

/**
 * Imprime todos os registros, presentes no arquivo de registros.
 */
void imprimirRegistrosArquivo(){
    FILE *arquivo = ABRIR_ARQUIVO_LEITURA();
    Registro registro_atual;
    int i;

    // Percore o arquivo de registros e imprime registro por registro do mesmo
    for(i = 0; i < MAXNUMREGS; i++){
        fseek(arquivo, (long)(i * TAMANHO_REGISTRO()), SEEK_SET);
        fread(&registro_atual, TAMANHO_REGISTRO(), 1, arquivo);

        printf("%d: ", i);
        if(registro_atual.chave == CHAVE_VAZIA) {
            printf("vazio\n");
        } else if (registro_atual.chave == CHAVE_REMOVIDA) {
            printf("*\n");
        } else {
            printf("%d %s %d\n", registro_atual.chave, registro_atual.nome, registro_atual.idade);
        }
    }

    fclose(arquivo);
}

/**
 * Consulta um registro no arquivo de registros.
 *
 * @param chave Chave a ser consultada.
 * @param registro_retorno Registro encontrado por uma consulta, use NULO se não desejar.
 * @param endereco_inserir Informa o primeiro endereço para inserção, use NULO se não desejar.
 * @return posição a qual a chave foi encontrada
 */
int consultarRegistro(unsigned int chave, Registro *registro_retorno, int *endereco_inserir) {
    FILE* arquivo = ABRIR_ARQUIVO_LEITURA();
    Registro registro_encontrado;
    int hash_computado;
    long endereco_hash;
    int i;

    for(i = 0; i < MAXNUMREGS; i++){
        hash_computado = h(i, chave);
        endereco_hash = (long)(hash_computado * TAMANHO_REGISTRO());

        fseek(arquivo, endereco_hash, SEEK_SET);
        fread(&registro_encontrado, TAMANHO_REGISTRO(), 1, arquivo);

        if(registro_encontrado.chave == chave){
            if (registro_retorno != NULL) {
                registro_retorno->chave = registro_encontrado.chave;
                registro_retorno->idade = registro_encontrado.idade;
                strcpy(registro_retorno->nome, registro_encontrado.nome);
            }

            if(
                endereco_inserir != NULL &&
                *endereco_inserir == CHAVE_INEXISTENTE){
                *endereco_inserir = CHAVE_EXISTENTE;
            }

            fclose(arquivo);

            return hash_computado;
        }
        else if(registro_encontrado.chave == CHAVE_VAZIA){
            if(
                endereco_inserir != NULL &&
                *endereco_inserir == CHAVE_INEXISTENTE
            ){
                *endereco_inserir = hash_computado;
            }

            break;
        }
        else if(
                endereco_inserir != NULL &&
                *endereco_inserir == CHAVE_INEXISTENTE &&
                registro_encontrado.chave == CHAVE_REMOVIDA
            ){
            *endereco_inserir = hash_computado;
        }
    }

    fclose(arquivo);

    return CHAVE_INEXISTENTE;
}

/**
 * Insere o registro no arquivo de registros.
 *
 * @param registro Registro a ser inserido.
 * @return Retorna o status da inserção do registro.
 */
int inserirRegistro(Registro registro){
    int posicao_inserir = ARQUIVO_CHEIO;
    consultarRegistro(registro.chave, NULL, &posicao_inserir);

    if(posicao_inserir == ARQUIVO_CHEIO || posicao_inserir == CHAVE_EXISTENTE){
        return posicao_inserir;
    }

    FILE* arquivo = ABRIR_ARQUIVO_LEITURA_ESCRITA();

    fseek(arquivo, (long)(posicao_inserir * TAMANHO_REGISTRO()), SEEK_SET);
    fwrite(&registro, TAMANHO_REGISTRO(), 1, arquivo);

    fclose(arquivo);
    return true;
}

/**
 * Remove o registro no arquivo de registros.
 *
 * @param chave Chave a ser removida.
 * @return Retorna true caso tenha sido removido e false caso não.
 */
bool removeRegistro(int chave){
    int posicao_remover = consultarRegistro(chave, NULL, NULL);

    if(posicao_remover == CHAVE_INEXISTENTE){
        return false;
    }

    Registro registro_deletado;
    registro_deletado.chave = CHAVE_REMOVIDA;
    registro_deletado.idade = 0;
    registro_deletado.nome[0] = '\0';

    FILE* arquivo = ABRIR_ARQUIVO_LEITURA_ESCRITA();

    fseek(arquivo, (long)(posicao_remover * TAMANHO_REGISTRO()), SEEK_SET);
    fwrite(&registro_deletado, TAMANHO_REGISTRO(), 1, arquivo);

    fclose(arquivo);
    return true;
}

/**
 * Retorna a média de acesso a todos os nós existentes no arquivo de registro.
 *
 * @return Media de acesso de acessos com sucesso e com falha.
 */
TuplaDouble mediaAcessoRegistros(){
    int i, j;
    unsigned int chave_desejada;

    int quantidade_de_registros = 0;
    int quantidade_de_registros_falha = 0;

    bool falha_acesso = false;

    double somatorio_acessos_sucesso = 0.0;
    double somatorio_acessos_falha = 0.0;
    FILE *arquivo = ABRIR_ARQUIVO_LEITURA();
    Registro registro_atual;

    for(i = 0; i < MAXNUMREGS; i++){
        fseek(arquivo, (long)(i * TAMANHO_REGISTRO()), SEEK_SET);
        fread(&registro_atual, TAMANHO_REGISTRO(), 1, arquivo);

        chave_desejada = registro_atual.chave;

        if(chave_desejada != CHAVE_INEXISTENTE && chave_desejada != CHAVE_REMOVIDA){
            rewind(arquivo);
            quantidade_de_registros++;
            falha_acesso = false;

            for ( j = 0; j < MAXNUMREGS; j++) {
                fseek(arquivo, (long)(h(j, chave_desejada) * TAMANHO_REGISTRO()), SEEK_SET);
                fread(&registro_atual, TAMANHO_REGISTRO(), 1, arquivo);

                somatorio_acessos_sucesso++;

                if (registro_atual.chave == chave_desejada) {
                    break;
                }

                falha_acesso = true;
                somatorio_acessos_falha++;
            }

            if (falha_acesso) {
                quantidade_de_registros_falha++;
            }
        }
    }

    fclose(arquivo);

    TuplaDouble ret;
    ret.a = quantidade_de_registros > 0 
        ? (somatorio_acessos_sucesso / quantidade_de_registros) 
        : 0.0;
    
    ret.b = quantidade_de_registros > 0 
        ? (somatorio_acessos_falha / quantidade_de_registros_falha) // TODO 
        : 0.0;

    return ret;
}

/* ----------------------------------------------------------------
 *                          Seção Principal
 * ---------------------------------------------------------------- */

int main(int argc, char *argv[]){
    long tamanho_arquivo = tamanhoArquivo();
    char opcao;
    int n_entradas;
    int i;

    Registro registro;

    int chave_entrada;
    int retorno_operacao;
    TuplaDouble media_acessos;

    // Checa se o arquivo existe
    if(tamanho_arquivo == ARQUIVO_INEXISTENTE || tamanho_arquivo != (TAMANHO_REGISTRO() * MAXNUMREGS)){
#ifdef DEBUG
        if (tamanho_arquivo == ARQUIVO_INEXISTENTE){
            printf(DEBUG_STRING "arquivo inexistente. Criando novo!\n");
        } else {
            printf(DEBUG_STRING "arquivo com tamanho inválido! Recriando arquivo!\n");
            printf(DEBUG_STRING "tamanho_desejado = %ld\ntamanho_arquivo = %ld\n",
                   MAXNUMREGS * TAMANHO_REGISTRO(), tamanho_arquivo);
        }
#endif

        criaArquivo();
    }

    do{
        scanf(" %c", &opcao);

        // Executa os comandos
        switch(opcao){
            case COMANDO_INSERIR_REGISTRO:
                scanf(" %d", &n_entradas);

                for (i = 0; i < n_entradas; i++) {
                    scanf(" %u", &registro.chave);
                    scanf(" %[^\n]s", registro.nome);
                    scanf("%*c");
                    scanf(" %u", &registro.idade);

                    retorno_operacao = inserirRegistro(registro);

                    if(retorno_operacao == CHAVE_EXISTENTE) {
                        printf("chave ja existente: %d\n", registro.chave);
                    } else if (retorno_operacao == ARQUIVO_CHEIO) {
                        printf("insercao de chave sem sucesso - arquivo cheio: %d\n", registro.chave);
                    } else {
                        printf("insercao com sucesso: %d\n", registro.chave);
                    }
                }

                break;

            case COMANDO_CONSULTAR_REGISTRO:
                scanf(" %d", &chave_entrada);

                if(consultarRegistro(chave_entrada, &registro, NULL) == CHAVE_INEXISTENTE){
                    printf("chave nao encontrada: %d\n", chave_entrada);
                }
                else{
                    printf("chave: ");
                    imprimirRegistro(registro);
                }

                break;

            case COMANDO_IMPRIMIR_NIVEL_1:
                break;

            case COMANDO_IMPRIMIR_NIVEL_2:
                printf("TODO!\n");
                break;

            case COMANDO_ESTRUTURA_GLOBAL:
                printf("TODO!\n");
                break;

            case COMANDO_CARDINALIDADE:
                printf("TODO!\n");
                break;

            case COMANDO_SAIR:
            default:
                break;
        }

    } while(opcao != COMANDO_SAIR);

    return EXIT_SUCCESS;
}
