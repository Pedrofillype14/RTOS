#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#define MAX_LINE_LENGTH 1024

volatile float valores[5] = {0,0,0,0,0};
volatile int numDeCidades[5] = {0,0,0,0,0};

typedef struct {
  pthread_mutex_t *lock;
  int totalLinhas[2];
  char **linhas;
  int *qtdPop;
} Dados;

void *threadPopReg (void *arg){
  Dados *dados = (Dados*)arg;
  char teste[32];
  int i;
  volatile float qntdPop[5] = {0,0,0,0,0};
  volatile int qntdCidade[5] = {0,0,0,0,0};

  //printf("%d %d\n", dados->totalLinhas[0], dados->totalLinhas[1]);

  for (i=dados->totalLinhas[0]; i<dados->totalLinhas[1]; i++){
    strcpy(teste, dados->linhas[i]);
    int k = 6; //decidir em qual regiao vai colocar as informacoes
               //1-CENTRO_OESTE / 2-NORTE / 3-NORDESTE / 4-SUL / 5_SUDESTE
    //printf("Auu[%d] = %s\n", i, teste);

    if (teste[8] == 'C') //Regiao centro-oeste
      k = 0;
    else if (teste[8] == 'N'){
      if (teste[11] == 't') //Regiao norte
        k = 1;
      else //Regiao nordeste
        k = 2;
    }
    else if (teste[10] == 'l') //Regiao sul
      k = 3;
    else if (teste[10] == 'd') //Regiao sudeste
      k = 4;

    qntdPop[k] += dados->qtdPop[i];
    qntdCidade[k]++;
  }

  for (i=0; i<5; i++) {
    pthread_mutex_lock(dados->lock);
    valores[i] = qntdPop[i];
    numDeCidades[i] = qntdCidade[i];
    pthread_mutex_unlock(dados->lock);
  }
  return NULL;
}

int main (int argc, char *argv[]) {
  clock_t begin = clock();
  FILE *arquivo;
  char linha[MAX_LINE_LENGTH];
  char *token;

  Dados *infoData = (Dados*)malloc(sizeof(Dados));
  Dados *duploData = (Dados*)malloc(sizeof(Dados));

  infoData->lock = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));

  infoData->totalLinhas[0] = 1;
  infoData->totalLinhas[1] = 0;

  pthread_mutex_init(infoData->lock, NULL);

  arquivo = fopen("dados_municipios.csv", "r");

  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo.\n");
    return 1;
  }

  while (fgets(linha, MAX_LINE_LENGTH, arquivo) != NULL)
    infoData->totalLinhas[1]++;

  infoData->linhas = (char **)malloc(infoData->totalLinhas[1] * sizeof(char *));
  infoData->qtdPop = (int *)malloc(infoData->totalLinhas[1] * sizeof(int));

  rewind(arquivo);
  int i;

  for (i=0; i<infoData->totalLinhas[1]; i++){
    infoData->linhas[i] = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));
    fgets(linha, MAX_LINE_LENGTH, arquivo);
    token = strtok(linha, ";");
    token = strtok(NULL, ";");
    token = strtok(NULL, ";");
    token = strtok(NULL, ";");
    token = strtok(NULL, ";");
    token = strtok(NULL, ";"); //Coluna referente a região

    strcpy(infoData->linhas[i], token);

    token = strtok(NULL, ";");
    infoData->qtdPop[i] = atoi(token);
  }

  int delta = 0;
  delta = infoData->totalLinhas[1]/2;

  infoData->totalLinhas[0] = infoData->totalLinhas[1] - delta;

  duploData->totalLinhas[1] = infoData->totalLinhas[0]-1;
  duploData->totalLinhas[0] = 1;

  //Igualando as threads
  duploData->lock = infoData->lock;
  duploData->linhas = infoData->linhas;
  duploData->qtdPop = infoData->qtdPop;

  //Iniciação das thread
  pthread_t t1, t2;
  pthread_create(&t1, NULL, threadPopReg, infoData);
  pthread_create(&t2, NULL, threadPopReg, duploData);

  pthread_join(t1, NULL);
  pthread_join(t2, NULL);

  clock_t end = clock();
  double tempoPercorrido = (double)(end-begin)/CLOCKS_PER_SEC;
  printf("Tempo de execução (Leitura + Execucao das 2 Threads):  %.6f segundos\n", tempoPercorrido);

  fclose(arquivo);

  for (i=0; i<infoData->totalLinhas[1]; i++)
    free(infoData->linhas[i]);

  free(infoData->qtdPop);
  free(infoData->linhas);
  free(infoData->lock);
  free(infoData);

  arquivo = fopen("populacao_regiao1.txt", "w");

  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo.\n");
    return 1;
  }

  fprintf(arquivo, "Região Centro-Oeste;%.2f\n", (valores[0]/numDeCidades[0]));
  fprintf(arquivo, "Região Norte;%.2f\n", (valores[1]/numDeCidades[1]));
  fprintf(arquivo, "Região Nordeste;%.2f\n", (valores[2]/numDeCidades[2]));
  fprintf(arquivo, "Região Sul;%.2f\n", (valores[3]/numDeCidades[3]));
  fprintf(arquivo, "Região Sudeste;%.2f\n", (valores[4]/numDeCidades[4]));

  fclose(arquivo);

  return 0;
}
