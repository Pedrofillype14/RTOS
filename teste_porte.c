#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

void ler_arquivo_csv(const char *nome_arquivo, float valor_anterior[], int num_por_porte[]);
void calcular_medias(float valor_anterior[], int num_por_porte[], float medias[]);
void escrever_arquivo_saida(const char *nome_arquivo, float medias[], int num_por_porte[]);

int main() {
    const char *input_file = "dados_municipios.csv";
    const char *output_file = "populacao_porte.txt";

    float valor_anterior[6] = {0.0}; // Valor acumulado de população para cada porte
    int num_por_porte[6] = {0};      // Número de cidades para cada porte
    float medias[6] = {0.0};         // Média da população para cada porte

    // Ler os dados do arquivo CSV
    ler_arquivo_csv(input_file, valor_anterior, num_por_porte);

    // Calcular as médias de população
    calcular_medias(valor_anterior, num_por_porte, medias);

    // Escrever os resultados no arquivo de saída
    escrever_arquivo_saida(output_file, medias, num_por_porte);

    return 0;
}

void ler_arquivo_csv(const char *nome_arquivo, float valor_anterior[], int num_por_porte[]) {
    FILE *arquivo;
    int populacao, i;

    // Abrir o arquivo de entrada
    arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de entrada.\n");
        exit(1);
    }

    // Ignorar a primeira linha (cabeçalhos das colunas)
    char linha[MAX_LINE_LENGTH];
    fgets(linha, MAX_LINE_LENGTH, arquivo);

    // Ler o arquivo linha por linha
    while (fgets(linha, MAX_LINE_LENGTH, arquivo) != NULL) {
        char *token = strtok(linha, ";");
        int coluna = 0;

        char *porte = NULL;

        while (token != NULL) {
            if (coluna == 6 && strlen(token) > 0) {
                populacao = atoi(token);
                porte = strtok(NULL, ";\n");

                if (porte == NULL) {
                    break; // Evita comportamento indefinido se porte for NULL
                }

                if (strcmp(porte, "Pequeno I") == 0) {
                    i = 1;
                } else if (strcmp(porte, "Pequeno II") == 0) {
                    i = 2;
                } else if (strncmp(porte, "Medio", 5) == 0) {
                    i = 3;
                } else if (strncmp(porte, "Grande", 6) == 0) {
                    i = 4;
                } else {
                    i = 5; // Metrópole
                }

                // Adicionar a população e contar o número de cidades para o porte correspondente
                valor_anterior[i] += populacao;
                num_por_porte[i]++;
            }

            token = strtok(NULL, ";");
            coluna++;
        }
    }

    // Fechar o arquivo de entrada
    fclose(arquivo);
}

void calcular_medias(float valor_anterior[], int num_por_porte[], float medias[]) {
    for (int i = 1; i < 6; i++) {
        if (num_por_porte[i] > 0) {
            medias[i] = valor_anterior[i] / num_por_porte[i];
        }
    }
}

void escrever_arquivo_saida(const char *nome_arquivo, float medias[], int num_por_porte[]) {
    FILE *arquivo_saida = fopen(nome_arquivo, "w");
    if (arquivo_saida == NULL) {
        printf("Erro ao criar o arquivo de saída.\n");
        exit(1);
    }

    // Escrever os resultados no arquivo
    if (num_por_porte[1] > 0)
        fprintf(arquivo_saida, "Pequeno I;%.2f\n", medias[1]);
    if (num_por_porte[2] > 0)
        fprintf(arquivo_saida, "Pequeno II;%.2f\n", medias[2]);
    if (num_por_porte[3] > 0)
        fprintf(arquivo_saida, "Medio;%.2f\n", medias[3]);
    if (num_por_porte[4] > 0)
        fprintf(arquivo_saida, "Grande;%.2f\n", medias[4]);
    if (num_por_porte[5] > 0)
        fprintf(arquivo_saida, "Metrópole;%.2f\n", medias[5]);

    // Fechar o arquivo de saída
    fclose(arquivo_saida);

    // Imprimir os valores
    if (num_por_porte[1] > 0)
        printf("Pequeno I: %.2f\n", medias[1]);
    if (num_por_porte[2] > 0)
        printf("Pequeno II: %.2f\n", medias[2]);
    if (num_por_porte[3] > 0)
        printf("Medio: %.2f\n", medias[3]);
    if (num_por_porte[4] > 0)
        printf("Grande: %.2f\n", medias[4]);
    if (num_por_porte[5] > 0)
        printf("Metrópole: %.2f\n", medias[5]);
}
