#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX_LINE_LENGTH 1024
#define NUM_ESTADOS 27

typedef struct {
    char estado[3];
    float populacao_total;
    int num_cidades;
} Estado;

void inicializar_estados(Estado estados[]);
void ler_arquivo_csv(const char *nome_arquivo, Estado estados[]);
void calcular_medias(Estado estados[], float medias[]);
void escrever_arquivo_saida(const char *nome_arquivo, Estado estados[], float medias[]);

int main() {
    const char *input_file = "dados_municipios.csv";
    const char *output_file = "populacao_estado.txt";

    Estado estados[NUM_ESTADOS];
    float medias[NUM_ESTADOS] = {0.0};

    // Inicializar os dados dos estados
    inicializar_estados(estados);

    // Ler os dados do arquivo CSV
    ler_arquivo_csv(input_file, estados);

    // Calcular as médias de população
    calcular_medias(estados, medias);

    // Escrever os resultados no arquivo de saída
    escrever_arquivo_saida(output_file, estados, medias);

    return 0;
}

void inicializar_estados(Estado estados[]) {
    const char* siglas[NUM_ESTADOS] = {
        "AC", "AL", "AM", "AP", "BA", "CE", "DF", "ES", "GO", "MA", "MG", "MS", "MT", "PA", "PB", "PE", "PI", "PR", "RJ", "RN", "RO", "RR", "RS", "SC", "SE", "SP", "TO"
    };

    for (int i = 0; i < NUM_ESTADOS; i++) {
        strcpy(estados[i].estado, siglas[i]);
        estados[i].populacao_total = 0.0;
        estados[i].num_cidades = 0;
    }
}

void ler_arquivo_csv(const char *nome_arquivo, Estado estados[]) {
    FILE *arquivo;
    long populacao;
    char linha[MAX_LINE_LENGTH];
    char estado[3];

    // Abrir o arquivo de entrada
    arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de entrada.\n");
        exit(1);
    }

    // Ignorar a primeira linha (cabeçalhos das colunas)
    fgets(linha, MAX_LINE_LENGTH, arquivo);

    // Ler o arquivo linha por linha
    while (fgets(linha, MAX_LINE_LENGTH, arquivo) != NULL) {
        char *token = strtok(linha, ";");
        int coluna = 0;

        while (token != NULL) {
            if (coluna == 3) {
                strncpy(estado, token, sizeof(estado) - 1);
                estado[sizeof(estado) - 1] = '\0';
            } else if (coluna == 6 && strlen(token) > 0) {
                errno = 0;
                populacao = strtol(token, NULL, 10);
                if (errno != 0) {
                    printf("Erro na conversão da população para inteiro: %s\n", token);
                    exit(1);
                }

                // Procurar o estado correspondente
                for (int i = 0; i < NUM_ESTADOS; i++) {
                    if (strcmp(estados[i].estado, estado) == 0) {
                        estados[i].populacao_total += populacao;
                        estados[i].num_cidades++;
                        break;
                    }
                }
            }

            token = strtok(NULL, ";");
            coluna++;
        }
    }

    // Fechar o arquivo de entrada
    fclose(arquivo);
}

void calcular_medias(Estado estados[], float medias[]) {
    for (int i = 0; i < NUM_ESTADOS; i++) {
        if (estados[i].num_cidades > 0) {
            medias[i] = estados[i].populacao_total / estados[i].num_cidades;
        }
    }
}

void escrever_arquivo_saida(const char *nome_arquivo, Estado estados[], float medias[]) {
    FILE *arquivo_saida = fopen(nome_arquivo, "w");
    if (arquivo_saida == NULL) {
        printf("Erro ao criar o arquivo de saída.\n");
        exit(1);
    }

    // Escrever os resultados no arquivo
    for (int i = 0; i < NUM_ESTADOS; i++) {
        if (estados[i].num_cidades > 0) {
            fprintf(arquivo_saida, "%s;%.2f\n", estados[i].estado, medias[i]);
        }
    }

    // Fechar o arquivo de saída
    fclose(arquivo_saida);

    printf("Resultados salvos em 'populacao_estado.txt'.\n");
}
