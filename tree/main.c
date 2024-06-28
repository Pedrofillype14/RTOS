#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    printf("Processo Pai (pid:%d)\n", (int) getpid());

    int pop_reg = fork();
    int pop_por = fork();
    int pop_est = fork();
    if (pop_reg < 0 || pop_por < 0 || pop_est < 0 ) {
        //Algum dos Forks falhou
        fprintf(stderr, "Fork Failed\n");
        exit(1);
    } else if (pop_reg == 0) {
        // Processo Filho - Populacao Regiao
        printf("Processo Filho de Populacao Regiao (pid:%d)\n", (int) getpid());
        char *myargs[2];
        myargs[0] = strdup("./teste_regiao");   // program: 
        myargs[1] = NULL;           // marks end of array
        execvp(myargs[0], myargs);  // runs word count
    } else if (pop_por == 0) {
        // Processo Filho - Populacao Porte
        printf("Processo Filho de Populacao Porte (pid:%d)\n", (int) getpid());
        char *myargs[2];
        myargs[0] = strdup("./teste_porte");   // program: 
        myargs[1] = NULL;           // marks end of array
        execvp(myargs[0], myargs);  // runs word count
    } else if (pop_est == 0) {
        // Processo Filho - Populacao Estado
        printf("Processo Filho de Populacao Estado (pid:%d)\n", (int) getpid());
        char *myargs[2];
        myargs[0] = strdup("./estado");   // program: 
        myargs[1] = NULL;           // marks end of array
        execvp(myargs[0], myargs);  // runs word count
    } else {
        // parent goes down this path (original process)
        int wstatus;
        //int ls = wait(&wstatus);
        int exec_pop_reg = waitpid(pop_reg, &wstatus, 0);
        int exec_pop_por = waitpid(pop_por, &wstatus, 0);
        int exec_pop_est = waitpid(pop_est, &wstatus, 0);
        printf("Sou Processo Pai %d %d %d (pop_reg:%d) (pop_por:%d) (pop_est:%d) (pid:%d) (status:%d)\n",
               pop_reg, pop_por, pop_est, exec_pop_reg, exec_pop_por, exec_pop_est, (int) getpid(), wstatus);
    }

    FILE *arquivo;
    char linha[1024];
    char *token;

    arquivo = fopen("populacao_regiao.txt", "r");
    if (arquivo == NULL) {
      printf("Erro ao abrir o arquivo.\n");
      return 1;
    }
    printf("\n\n==== Media por Região ====\n");

    while (fgets(linha, 1024, arquivo) != NULL) {
      token = strtok(linha, ";");
      printf("%s = ", token); 
      token = strtok(NULL, ";");    
      printf("%s", token);
    }

    fclose(arquivo);

    arquivo = fopen("populacao_porte.txt", "r");
    if (arquivo == NULL) {
      printf("Erro ao abrir o arquivo.\n");
      return 1;
    }

    printf("\n\n==== Media por Porte ====\n");

    while (fgets(linha, 1024, arquivo) != NULL) {
      token = strtok(linha, ";");
      printf("%s = ", token);
      token = strtok(NULL, ";");
      printf("%s", token);
    }

    fclose(arquivo);

    arquivo = fopen("populacao_estado.txt", "r");
    if (arquivo == NULL) {
      printf("Erro ao abrir o arquivo.\n");
      return 1;
    }

    printf("\n\n==== Media por Estado ====\n");

    while (fgets(linha, 1024, arquivo) != NULL) {
      token = strtok(linha, ";");
      printf("%s = ", token);
      token = strtok(NULL, ";");
      printf("%s", token);
    }

    fclose(arquivo);

    return 0;
}