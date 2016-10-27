/*
// Projeto SO - exercicio 2, version 1
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
#include <pthread.h>
#include <semaphore.h>

#define COMANDO_DEBITAR "debitar"
#define COMANDO_CREDITAR "creditar"
#define COMANDO_LER_SALDO "lerSaldo"
#define COMANDO_SIMULAR "simular"
#define COMANDO_SAIR "sair"
#define COMANDO_SAIR_AGORA "agora"

#define OP_DEBITAR 1
#define OP_CREDITAR 2
#define OP_LERSALDO 3
#define OP_SAIR 0

#define MAXARGS 3
#define BUFFER_SIZE 100

#define MAX_CHILDREN 20

#define NUM_TRABALHADORAS 3
#define CMD_BUFFER_DIM (NUM_TRABALHADORAS * 2)

typedef struct {
		int operacao;
		int idConta;
		int valor;
} comando_t;

void funcaoSaida(int nFilhos);
void enviarSignal(int pidFilhos[], int nFilhos);
void criaPoolTarefas();

void cria_trabalho(int oper, int accountID, int moneyValue);
void* tarefa_trabalhadora(void *dummy);
void realiza_trabalho(comando_t trabalho);

pthread_mutex_t trinco_write;
pthread_mutex_t trinco_read;
pthread_mutex_t trincos_contas[NUM_CONTAS];

int buff_write_idx = 0, buff_read_idx = 0;

sem_t sem_write;
sem_t sem_read;

comando_t cmd_buffer[CMD_BUFFER_DIM];

pthread_t tid[NUM_TRABALHADORAS];

int main (int argc, char** argv) {

	char *args[MAXARGS + 1];
	char buffer[BUFFER_SIZE];

	int pidFilhos[MAX_CHILDREN];
	int nFilhos = 0;  /* numero de processos filho criados */

	int i;

/* --- inicializacao dos mutexes e dos semaforos --- */
	pthread_mutex_init(&(trinco_write), NULL);
	pthread_mutex_init(&(trinco_read), NULL);

	sem_init(&sem_write, 0, CMD_BUFFER_DIM);
	sem_init(&sem_read, 0, 0);

	for (i = 0; i < NUM_CONTAS; ++i) {
		
		pthread_mutex_init(&(trincos_contas[i]), NULL);
	}
	
	criaPoolTarefas();

	inicializarContas();

	printf("Bem-vinda/o ao i-banco\n\n");
			
	while (1) {
		
		int numargs;
		
		numargs = readLineArguments(args, MAXARGS+1, buffer, BUFFER_SIZE);

		/* EOF (end of file) do stdin ou comando "sair" */
		if (numargs < 0 ||
		   (numargs > 0 && (strcmp(args[0], COMANDO_SAIR) == 0))) {

			printf("i-banco vai terminar.\n--\n");

			/* Sair Agora */
			if ((args[1] != NULL) && (strcmp(args[1], COMANDO_SAIR_AGORA)) == 0)
				enviarSignal(pidFilhos, nFilhos);			
	
			for (i = 0; i < NUM_TRABALHADORAS; i++) 
				cria_trabalho(OP_SAIR, 0, 0);	/* no comando sair e' mandado o comando de saida a cada thread,			   */
												/* enviando para o buffer tantos comandos de saida quantas threads existem */
			
			for (i = 0; i < NUM_TRABALHADORAS; i++)
				pthread_join((tid[i]), NULL);   /* de seguida fica 'a espera que todas as threads terminem antes de sair do programa. */
			
			funcaoSaida(nFilhos);
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

			cria_trabalho(OP_DEBITAR, idConta, valor);
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

			cria_trabalho(OP_CREDITAR, idConta, valor);
		}

		/* Ler Saldo */
		else if (strcmp(args[0], COMANDO_LER_SALDO) == 0) {
			
			int idConta;

			if (numargs < 2) {
				
				printf("%s: Sintaxe inválida, tente de novo.\n", COMANDO_LER_SALDO);
				continue;
			}

			idConta = atoi(args[1]);

			cria_trabalho(OP_LERSALDO, idConta, 0);
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

				if (pid == 0) {
					
					simular(numAnos);
					exit(EXIT_SUCCESS); /* retorna ao processo pai */
				}

				else 
					pidFilhos[nFilhos++] = pid;
			}
		}

		else 
			printf("Comando desconhecido. Tente de novo.\n");
	} 
}

void funcaoSaida(int nFilhos) {

	int i = 0, j = 0, pid, estado; 

	int pids_sucess[MAX_CHILDREN];
	int pids_failure[MAX_CHILDREN];

	while ((i+j) < (nFilhos)) { 
			
		/* espera pelo fim de cada processo filho */
		pid = wait(&estado);
			
		/* Os PID's dos processos filhos são guardados num vetor correspondente ao sucesso desse processo na terminação */			
		if (WIFEXITED(estado)) {
			
			pids_sucess[i] = pid;
			i++;
		}
			
		else {
			
			pids_failure[j] = pid;
			j++;
		}
	}

	while (i != 0) {
		
		i--;
		printf("FILHO TERMINADO (PID=%d; terminou normalmente)\n", pids_sucess[i]);
	}
	
	while (j != 0) {
		
		j--;
		printf("FILHO TERMINADO (PID=%d; terminou abruptamente)\n", pids_failure[j]);
	}

	printf("--\n");
	printf("i-banco terminou.\n");
}

/* Envia um signal a todos os processos criados */
void enviarSignal(int pidFilhos[], int nFilhos) {
		int i;

		for (i = 0; i < nFilhos; i++) 
				kill(pidFilhos[i], SIGUSR1);
}

/* Cria uma pool de NUM_TRABALHADORAS tarefas e associa-as 'a tarefa_trabalhadora */
void criaPoolTarefas() {
	int i;

	for (i = 0; i < NUM_TRABALHADORAS; i++)
	{
		if (pthread_create(&tid[i], NULL, tarefa_trabalhadora, NULL) != 0)
		{
			printf("Erro.\n");
			exit(EXIT_FAILURE);
		}
	}
}

/* 	A cada instrucao recebida pelo stdin, e' criado um trabalho com a respectiva operacao. 
	A criacao do trabalho consiste em colocar a operacao no buffer (cmd_buffer) na forma de comando_t, de modo 
	a ser posteriormente executada. 
	Funciona como um produtor. */
void cria_trabalho(int oper, int accountID, int moneyValue) {

	comando_t trabalho;
	trabalho.operacao = oper;
	trabalho.idConta = accountID;
	trabalho.valor = moneyValue;

	/* Consome uma vaga para a escrita no buffer. Se nao houver vagas, bloqueia-se e espera que haja. */
	sem_wait(&sem_write);

	/* ----- A operacao e' colocada no buffer em exclusao mutua. ----- */
	pthread_mutex_lock(&trinco_write);

	cmd_buffer[buff_write_idx] = trabalho;
	buff_write_idx = (buff_write_idx + 1) % CMD_BUFFER_DIM;

	pthread_mutex_unlock(&trinco_write); 
	/* --------------------- Fim de exclusao mutua ------------------- */
	
	/* Assinala aos consumidores que ha operacoes para serem executadas. */
	sem_post(&sem_read);
}

/*Le os comandos colocados no buffer e realiza as operacoes associadas (via realiza_trabalho).
	Funciona como um consumidor.  */
void* tarefa_trabalhadora(void *dummy) {

	while(1) {

		comando_t trabalho;
		
		/* Consome uma vaga para leitura do buffer. Se nao houver vagas, bloqueia-se e espera que haja. */
		sem_wait(&sem_read);

	/* ----- Leitura dos comandos a partir do buffer feita em exclusao mutua -----*/
		pthread_mutex_lock(&trinco_read);

		trabalho = cmd_buffer[buff_read_idx];
		buff_read_idx = (buff_read_idx + 1) % CMD_BUFFER_DIM;

		pthread_mutex_unlock(&trinco_read); 
	/* ---------------------- Fim de exclusao mutua ----------------------------- */

		/* Assinala aos produtores que e' possivel escrever uma operacao no buffer */
		sem_post(&sem_write);

		realiza_trabalho(trabalho);
	}

	return NULL;
}

/* Funcao auxiliar a tarefa_trabalhadora. 
	Realiza a operacao propriamente dita associada ao comando lido. 
	Cada operacao e' realizada em exclusao mutua. */
void realiza_trabalho(comando_t trabalho) {

	int oper = trabalho.operacao;
	int idConta = trabalho.idConta;
	int valor = trabalho.valor;
	int saldo;

	switch(oper) {

		case OP_CREDITAR:

			pthread_mutex_lock(&(trincos_contas[idConta-1]));

			if (creditar (idConta, valor) < 0)
				printf("%s(%d, %d): Erro\n\n", COMANDO_CREDITAR, idConta, valor);
			else
				printf("%s(%d, %d): OK\n\n", COMANDO_CREDITAR, idConta, valor);

			pthread_mutex_unlock(&(trincos_contas[idConta-1]));
			break;

		case OP_DEBITAR:
			
			pthread_mutex_lock(&(trincos_contas[idConta-1]));

			if (debitar (idConta, valor) < 0)
				printf("%s(%d, %d): Erro\n\n", COMANDO_DEBITAR, idConta, valor);
			else
				printf("%s(%d, %d): OK\n\n", COMANDO_DEBITAR, idConta, valor);

			pthread_mutex_unlock(&(trincos_contas[idConta-1]));
			break;

		case OP_LERSALDO:
			
			pthread_mutex_lock(&(trincos_contas[idConta-1]));

			saldo = lerSaldo (idConta);
			if (saldo < 0)
				printf("%s(%d): Erro.\n\n", COMANDO_LER_SALDO, idConta);
			else 
				printf("%s(%d): O saldo da conta é %d.\n\n", COMANDO_LER_SALDO, idConta, saldo);

			pthread_mutex_unlock(&(trincos_contas[idConta-1]));
			break;

		case OP_SAIR:
			
			pthread_exit(NULL);
			break;

		default:
			printf("Erro: valor %d desconhecido.\n", oper);
	}
}