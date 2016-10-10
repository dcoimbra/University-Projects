/*
// Projeto SO - exercicio 1, version 1
// Sistemas Operativos, DEI/IST/ULisboa 2016-17
*/

#include "commandlinereader.h"
#include "contas.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


#define COMANDO_DEBITAR "debitar"
#define COMANDO_CREDITAR "creditar"
#define COMANDO_LER_SALDO "lerSaldo"
#define COMANDO_SIMULAR "simular"
#define COMANDO_SAIR "sair"
#define COMANDO_SAIR_AGORA "agora"


#define MAX_CHILDREN 20
#define MAXARGS 3
#define BUFFER_SIZE 100

void funcaoSaida();
void enviarSignal();

int pidFilhos[MAX_CHILDREN];
int nFilhos = 0; /* numero de processos filho criados */

int main (int argc, char** argv) {

    char *args[MAXARGS + 1];
    char buffer[BUFFER_SIZE];
    

    inicializarContas();

    printf("Bem-vinda/o ao i-banco\n\n");
      
    while (1) {
        int numargs;
    
        numargs = readLineArguments(args, MAXARGS+1, buffer, BUFFER_SIZE);

        /* EOF (end of file) do stdin ou comando "sair" */
        if (numargs < 0 ||
	        (numargs > 0 && (strcmp(args[0], COMANDO_SAIR) == 0))) {


        /* Sair Agora */
            if ((args[1] != NULL) && (strcmp(args[1], COMANDO_SAIR_AGORA)) == 0) {
                enviarSignal();
            }
            
            funcaoSaida();
            exit(EXIT_SUCCESS);
        }
    
        else if (numargs == 0)
            /* Nenhum argumento; ignora e volta a pedir */
            continue;
            
        /* Debitar */
        else if (strcmp(args[0], COMANDO_DEBITAR) == 0) {
            int idConta, valor;
            if (numargs < 3) {
                printf("%s: Sintaxe inválida, tente de novo.\n", COMANDO_DEBITAR);
               continue;
            }

            idConta = atoi(args[1]);
            valor = atoi(args[2]);

            if (debitar (idConta, valor) < 0)
               printf("%s(%d, %d): Erro\n\n", COMANDO_DEBITAR, idConta, valor);
            else
                printf("%s(%d, %d): OK\n\n", COMANDO_DEBITAR, idConta, valor);
        }

        /* Creditar */
        else if (strcmp(args[0], COMANDO_CREDITAR) == 0) {
            int idConta, valor;
            if (numargs < 3) {
                printf("%s: Sintaxe inválida, tente de novo.\n", COMANDO_CREDITAR);
                continue;
            }

            idConta = atoi(args[1]);
            valor = atoi(args[2]);

            if (creditar (idConta, valor) < 0)
                printf("%s(%d, %d): Erro\n\n", COMANDO_CREDITAR, idConta, valor);
            else
                printf("%s(%d, %d): OK\n\n", COMANDO_CREDITAR, idConta, valor);
        }

        /* Ler Saldo */
        else if (strcmp(args[0], COMANDO_LER_SALDO) == 0) {
            int idConta, saldo;

            if (numargs < 2) {
                printf("%s: Sintaxe inválida, tente de novo.\n", COMANDO_LER_SALDO);
                continue;
            }
            idConta = atoi(args[1]);
            saldo = lerSaldo (idConta);
            if (saldo < 0)
                printf("%s(%d): Erro.\n\n", COMANDO_LER_SALDO, idConta);
            else
                printf("%s(%d): O saldo da conta é %d.\n\n", COMANDO_LER_SALDO, idConta, saldo);
        }

        /* Simular */
        else if (strcmp(args[0], COMANDO_SIMULAR) == 0) {
            int numAnos, pid;

            if (numargs < 2) {
                printf("%s: Sintaxe inválida, tente de novo.\n", COMANDO_SIMULAR);
                continue;
            }
            numAnos = atoi(args[1]);

            if (numAnos < 0)
                printf("%s(%d): Erro.\n\n", COMANDO_SIMULAR, numAnos);

            else {
                pid = fork();

            /* Processo Filho */
                if (pid == 0) {
                    simular(numAnos);
                }

            /* Processo Pai */
                else {
                    pidFilhos[nFilhos++] = pid;
                }
            }
        }

        else {
          printf("Comando desconhecido. Tente de novo.\n");
        }
    }
}


void funcaoSaida() {

  int i = 0, j = 0, pid, estado; 

  int pids_sucess[MAX_CHILDREN];
  int pids_failure[MAX_CHILDREN];

  printf("i-banco vai terminar.\n");
  printf("--\n");

  while ((i+j) < (nFilhos)) { 

      /* espera pelo fim de cada processo filho */
      pid = wait(&estado);
    
      /* Os PID's dos processos filhos 
         são guardados no vetor correspondente
         ao sucesso desse processo na terminação */
      
      if (!WEXITSTATUS(estado)) {
          pids_sucess[i] = pid;
          i++;
      }
      
      else {
          pids_failure[j] = pid;
          j++;
      }
  }

  while (i != 0) {
    i--;    /* porque o i (e o j) e' sempre incrementado depois de se meter o pid no vector. */
    printf("FILHO TERMINADO (PID=%d; terminou normalmente)\n", pids_sucess[i]);
  }

  while (j != 0) {
    j--;
    printf("FILHO TERMINADO (PID=%d; terminou abruptamente)\n", pids_failure[j]);
  }

  printf("--\n");
  printf("i-banco terminou.\n");
}

void enviarSignal() {
    int i;

    for (i = 0; i < nFilhos; i++) {
        kill(pidFilhos[i], SIGUSR1);
    }
}