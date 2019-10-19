/*
// Projeto SO - exercicio 4, version 1
// Sistemas Operativos, DEI/IST/ULisboa 2016-17
*/

#include "i-banco.h"
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
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

void funcaoSaida(int nFilhos);
void enviarSignal(int pidFilhos[], int nFilhos);
void criaPoolTarefas();

void cria_trabalho(comando_t trabalho);
void* tarefa_trabalhadora(void *dummy);
void realiza_trabalho(comando_t trabalho);

pthread_mutex_t trinco_write;
pthread_mutex_t trinco_read;
pthread_mutex_t trincos_contas[NUM_CONTAS];

sem_t sem_write;
sem_t sem_read;

pthread_mutex_t count_mutex;
pthread_cond_t  count_cond;
int count = 0;

int buff_write_idx = 0, buff_read_idx = 0;

comando_t cmd_buffer[CMD_BUFFER_DIM];

pthread_t tid[NUM_TRABALHADORAS];

/* Descritor do ficheiro para onde escrever o log*/
int logFileDescriptor;

int main (int argc, char** argv) {

	pid_t pidFilhos[MAX_CHILDREN];
	pid_t pid;

	int nFilhos = 0;  
	int i, fdLeitura;
	int operacao;
	int numAnos;
	
	char* fifoLeitura = "/tmp/i-banco-pipe";
	char simFileName[MAX_STR_SIZE];

	if (pthread_mutex_init(&(trinco_write), NULL) != 0) {
		
		perror ("pthread_mutex_init(trinco_write)");
		exit(EXIT_FAILURE);
	}
	
	if (pthread_mutex_init(&(trinco_read), NULL) != 0) {

		perror ("pthread_mutex_init(trinco_read)");
		exit(EXIT_FAILURE);
	}

	if (sem_init(&sem_write, 0, CMD_BUFFER_DIM) == -1) {
		
		perror ("sem_init(sem_write)");
		exit(EXIT_FAILURE);
	}
	
	if (sem_init(&sem_read, 0, 0) == -1) {
		
		perror ("sem_init(sem_read)");
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < NUM_CONTAS; ++i) {
		
		if (pthread_mutex_init(&(trincos_contas[i]), NULL) != 0) {

			perror ("pthread_mutex_init(trincos_contas)");
			exit(EXIT_FAILURE);
		}
	}

	if (pthread_mutex_init(&count_mutex, NULL) != 0) {

		perror ("pthread_mutex_init(count_mutex)");
		exit(EXIT_FAILURE);
	}
	
	if (pthread_cond_init(&count_cond, NULL) != 0) {

		perror ("pthread_cond_init(count_cond)");
		exit(EXIT_FAILURE);
	}

	if (signal(SIGUSR1, tratarSignal) == SIG_ERR) { /* Faz o handle do signal e indica*/
    												/* caso haja erro */
    	perror ("signal(SIGUSR1)");
    	exit(EXIT_FAILURE);
    }

	logFileDescriptor = open("log.txt", O_CREAT | O_WRONLY | O_APPEND, 0666);
	
	if (logFileDescriptor == -1) {
		
		perror("open(log.txt)");
		exit(EXIT_FAILURE);
	}
	
	criaPoolTarefas();

	inicializarContas();

	if (mkfifo(fifoLeitura, 0666) != 0) {
		
		perror ("mkfifo(fifoLeitura)");
	}

	fdLeitura = open(fifoLeitura, O_RDONLY);

	if (fdLeitura == -1) {
		
		perror ("open(fifoLeitura)");
		unlink(fifoLeitura);
		exit(EXIT_FAILURE);
	}
			
	while (1) {

		comando_t trabalho;

		int sizeoft;
			
		sizeoft = read(fdLeitura, &trabalho, sizeof(trabalho));
	    
		if (sizeoft == -1) {

			perror ("read(fdLeitura)");
			close(fdLeitura);
			unlink(fifoLeitura);
			exit(EXIT_FAILURE); 
		}

	    if (sizeoft == 0) {
	    	
	    	if (close(fdLeitura) == -1) {

	    		perror ("close(fdLeitura)");
				unlink(fifoLeitura);
	    		exit(EXIT_FAILURE);
	    	}
	    	
	    	fdLeitura = open(fifoLeitura, O_RDONLY);

	    	if (fdLeitura == -1) {
	    		
	    		perror ("open(fifoLeitura)");
				unlink(fifoLeitura);
	    		exit(EXIT_FAILURE);
	    	}

	    	continue;
	    }

	    operacao = trabalho.operacao;

		switch (operacao) {
		
		case OP_SAIR_AGORA:

			enviarSignal(pidFilhos, nFilhos);
			trabalho.operacao = OP_SAIR;

		case OP_SAIR:

			printf("i-banco vai terminar.\n--\n");

			for (i = 0; i < NUM_TRABALHADORAS; i++) {
				
				cria_trabalho(trabalho);
			}
			
			for (i = 0; i < NUM_TRABALHADORAS; i++) {
				
				if (pthread_join((tid[i]), NULL) != 0) {

					perror ("pthread_join(tid)");
					exit(EXIT_FAILURE);
				}
			}

			if( close(fdLeitura) == -1) {
				
				perror("close(fdLeitura");
			}
			
			if (unlink(fifoLeitura) == -1) {

				perror ("unlink(fifoLeitura)");
				exit(EXIT_FAILURE);
			}
			
			funcaoSaida(nFilhos);
			exit(EXIT_SUCCESS);
			
			break;
		
		case OP_DEBITAR:
		case OP_CREDITAR:		
		case OP_LERSALDO:
		case OP_TRANSFERIR:

			cria_trabalho(trabalho);
			
			break;

		case OP_SIMULAR:

			numAnos = trabalho.valor;
			
			if (pthread_mutex_lock(&count_mutex) != 0) {

				perror ("pthread_mutex_lock(count_mutex)");
				close(fdLeitura);
				unlink(fifoLeitura);
				exit(EXIT_FAILURE);
			}

			while (count != 0) {
				
				if (pthread_cond_wait(&count_cond, &count_mutex) != 0) {
					
					perror ("pthread_cond_wait(count_cond, count_mutex)");
					close(fdLeitura);
					unlink(fifoLeitura);
					exit(EXIT_FAILURE);
				}
			}

			if ((pid = fork()) == -1) {
				
				perror ("fork()");
				close(fdLeitura);
				unlink(fifoLeitura);
				exit(EXIT_FAILURE);
			}

			if (pid == 0) {

				int sim_fd;

				if (close(fdLeitura) == -1) {

					perror("close(fdLeitura");
					exit(EXIT_FAILURE);
				}
				
				snprintf(simFileName, MAX_STR_SIZE, "i-banco-sim-%d.txt", getpid());

				sim_fd = open(simFileName, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

				if (sim_fd == -1) {
					
					perror("open(simFileName)");
					exit(EXIT_FAILURE);
				}

				if (dup2(sim_fd, STDOUT_FILENO) == -1) {
					
					perror("dup2(sim_fd, STDOUT_FILENO)");
					close(sim_fd);
					exit(EXIT_FAILURE);
				}

				simular(numAnos);

				if (close(sim_fd) == -1) {
					
					perror("close(sim_fd)");
					exit(EXIT_FAILURE);
				}
				
				exit(EXIT_SUCCESS);
			}

			else 
				pidFilhos[nFilhos++] = pid;
			
			if (pthread_mutex_unlock(&count_mutex) != 0) {
				
				perror ("pthread_mutex_unlock(count_mutex)");
				close(fdLeitura);
				unlink(fifoLeitura);
				exit(EXIT_FAILURE);
			}
		
		break;

		default: 
			
			printf("Erro: comando desconhecido.\n");
		}
	} 
}

void funcaoSaida(int nFilhos) {

	int i = 0, j = 0, pid, estado; 

	pid_t pids_sucess[MAX_CHILDREN];
	pid_t pids_failure[MAX_CHILDREN];

	while ((i+j) < (nFilhos)) {
		
		if ((pid = wait(&estado)) == -1) {

			perror ("wait()");
			exit(EXIT_FAILURE);
		}
					
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
void enviarSignal(pid_t pidFilhos[], int nFilhos) {

	int i;

	for (i = 0; i < nFilhos; i++) { 

		kill(pidFilhos[i], SIGUSR1);
	}
}

/* Cria uma pool de NUM_TRABALHADORAS tarefas e associa-as 'a tarefa_trabalhadora */
void criaPoolTarefas() {
	
	int i;

	for (i = 0; i < NUM_TRABALHADORAS; i++) {
		
		if (pthread_create(&tid[i], NULL, tarefa_trabalhadora, NULL) != 0) {
			
			perror("pthread_create(tid, tarefa_trabalhadora)");
			exit(EXIT_FAILURE);
		}
	}
}

/* 	A cada instrucao recebida pelo stdin, e' criado um trabalho com a respectiva operacao. 
	A criacao do trabalho consiste em colocar a operacao no buffer (cmd_buffer) na forma de comando_t, de modo 
	a ser posteriormente executada. Funciona como um produtor. */
void cria_trabalho(comando_t trabalho) {

	if (sem_wait(&sem_write) == -1) {

		perror ("sem_wait(sem_write)");
		exit(EXIT_FAILURE);
	}

	if (pthread_mutex_lock(&trinco_write) != 0) {

		perror ("pthread_mutex_lock(trinco_write)");
		exit(EXIT_FAILURE);
	}

	cmd_buffer[buff_write_idx] = trabalho;
	buff_write_idx = (buff_write_idx + 1) % CMD_BUFFER_DIM;

	if (pthread_mutex_lock(&count_mutex) != 0) {

		perror ("pthread_mutex_lock(count_mutex)");
		exit(EXIT_FAILURE);
	}
	
	count++;

	if (pthread_mutex_unlock(&count_mutex) != 0) {
		
		perror ("pthread_mutex_unlock(count_mutex)");
		exit(EXIT_FAILURE);
	}

	if (pthread_mutex_unlock(&trinco_write) != 0) {

		perror ("pthread_mutex_unlock(trinco_write)");
		exit(EXIT_FAILURE);
	} 
	
	if (sem_post(&sem_read) == -1) {
		
		perror ("sem_post(sem_read)");
		exit(EXIT_FAILURE);
	}
}

/*Le os comandos colocados no buffer e realiza as operacoes associadas (via realiza_trabalho). Funciona como um consumidor.  */
void* tarefa_trabalhadora(void *dummy) {

	while(1) {

		comando_t trabalho;
		
		if (sem_wait(&sem_read) == -1) {
			
			perror ("sem_wait(sem_read)");
			exit(EXIT_FAILURE);
		}
	
		if (pthread_mutex_lock(&trinco_read) != 0) {

			perror ("pthread_mutex_lock(trinco_read)");
			exit(EXIT_FAILURE);
		}

		trabalho = cmd_buffer[buff_read_idx];
		buff_read_idx = (buff_read_idx + 1) % CMD_BUFFER_DIM;

		if (pthread_mutex_unlock(&trinco_read) != 0) {

			perror ("pthread_mutex_unlock(trinco_read)");
			exit(EXIT_FAILURE);
		}
	
		if (sem_post(&sem_write) == -1) {
			
			perror("sem_post(sem_write)");
			exit(EXIT_FAILURE);
		}

		realiza_trabalho(trabalho);

		if (pthread_mutex_lock(&count_mutex) != 0) {
			
			perror ("pthread_mutex_lock(count_mutex)");
			exit(EXIT_FAILURE);
		}

		count--;

		if (pthread_cond_signal(&count_cond) != 0) {

			perror ("pthread_cond_signal(count_cond)");
			exit(EXIT_FAILURE);
		}

		if  (pthread_mutex_unlock(&count_mutex) != 0) {

			perror ("pthread_mutex_unlock(count_mutex)");
			exit(EXIT_FAILURE);
		}
	}

	return NULL;
}

/* Funcao auxiliar a tarefa_trabalhadora. Realiza a operacao propriamente dita associada ao comando lido. 
	Cada operacao e' realizada em exclusao mutua. */
void realiza_trabalho(comando_t trabalho) {

	int oper = trabalho.operacao;
	int idConta = trabalho.idConta1;
	int idConta2 = trabalho.idConta2;
	int valor = trabalho.valor;

	int bufSize = -1;
	int fdE;
	int saldo;
	int toWrite;

	char* fifoEscrita = trabalho.fifoL;
	char buf[MAX_BUF];
	char logBuffer[MAX_STR_SIZE];
	
	pthread_t tid = pthread_self();

	switch(oper) {

	case OP_CREDITAR:
		
		if (pthread_mutex_lock(&(trincos_contas[idConta - 1])) != 0) {

			perror ("pthread_mutex_lock(trincos_contas)");
			exit(EXIT_FAILURE);
		}

		if (creditar (idConta, valor) < 0) 
			bufSize = snprintf(buf, MAX_BUF, "%s(%d, %d): Erro\n\n", COMANDO_CREDITAR, idConta, valor);
		
		else 
			bufSize = snprintf(buf, MAX_BUF, "%s(%d, %d): OK\n\n", COMANDO_CREDITAR, idConta, valor);

		toWrite = snprintf(logBuffer, MAX_STR_SIZE, "%lu: %s %d %d\n", tid, COMANDO_CREDITAR, trabalho.idConta1, trabalho.valor);
		
		if (pthread_mutex_unlock(&(trincos_contas[idConta - 1])) != 0) {

			perror ("pthread_mutex_unlock(trincos_contas)");
			exit(EXIT_FAILURE);
		}

		break;

	case OP_DEBITAR:
		
		if (pthread_mutex_lock(&(trincos_contas[idConta - 1])) != 0) {

			perror ("pthread_mutex_lock(trincos_contas)");
			exit(EXIT_FAILURE);
		}

		if (debitar (idConta, valor) < 0) 
			bufSize = snprintf(buf, MAX_BUF, "%s(%d, %d): Erro\n\n", COMANDO_DEBITAR, idConta, valor);
		
		else 
			bufSize = snprintf(buf, MAX_BUF, "%s(%d, %d): OK\n\n", COMANDO_DEBITAR, idConta, valor);

		toWrite = snprintf(logBuffer, MAX_STR_SIZE, "%lu: %s %d %d\n", tid, COMANDO_DEBITAR, trabalho.idConta1, trabalho.valor);
		
		if (pthread_mutex_unlock(&(trincos_contas[idConta - 1])) != 0) {

			perror ("pthread_mutex_unlock(trincos_contas)");
			exit(EXIT_FAILURE);
		}
		
		break;

	case OP_LERSALDO:
		
		if (pthread_mutex_lock(&(trincos_contas[idConta - 1])) != 0) {

			perror ("pthread_mutex_lock(trincos_contas)");
			exit(EXIT_FAILURE);
		}

		saldo = lerSaldo (idConta);
		
		if (saldo < 0)
			bufSize = snprintf(buf, MAX_BUF, "%s(%d): Erro.\n\n", COMANDO_LER_SALDO, idConta);
		
		else 
			bufSize = snprintf(buf, MAX_BUF, "%s(%d): O saldo da conta é %d.\n\n", COMANDO_LER_SALDO, idConta, saldo);

		toWrite = snprintf(logBuffer, MAX_STR_SIZE, "%lu: %s %d\n", tid, COMANDO_LER_SALDO, trabalho.idConta1);
		
		if (pthread_mutex_unlock(&(trincos_contas[idConta - 1])) != 0) {

			perror ("pthread_mutex_unlock(trincos_contas)");
			exit(EXIT_FAILURE);
		}
		
		break;

	case OP_TRANSFERIR:
		
		if (pthread_mutex_lock((idConta < idConta2 ? &(trincos_contas[idConta - 1]) : &(trincos_contas[idConta2 - 1]))) != 0) {
			
			perror ("pthread_mutex_lock(trincos_contas)");
			exit(EXIT_FAILURE);
		}
		
		if (pthread_mutex_lock((idConta > idConta2 ? &(trincos_contas[idConta - 1]) : &(trincos_contas[idConta2 - 1]))) != 0) {

			perror ("pthread_mutex_lock(trincos_contas)");
			exit(EXIT_FAILURE);
		}

		if (transferir (idConta, idConta2, valor) < 0) 
			bufSize = snprintf(buf, MAX_BUF, "Erro ao %s valor da conta %d para a conta %d.\n\n", COMANDO_TRANSFERIR, idConta, idConta2);
		
		else 
			bufSize = snprintf(buf, MAX_BUF, "%s(%d, %d, %d): OK\n\n", COMANDO_TRANSFERIR, idConta, idConta2, valor);

		toWrite = snprintf(logBuffer, MAX_STR_SIZE, "%lu: %s %d %d %d\n", tid, COMANDO_TRANSFERIR, trabalho.idConta1, trabalho.idConta2, trabalho.valor);
		
		if (pthread_mutex_unlock((idConta > idConta2 ? &(trincos_contas[idConta - 1]) : &(trincos_contas[idConta2 - 1]))) != 0) {

			perror ("pthread_mutex_lock(trincos_contas)");
			exit(EXIT_FAILURE);
		}
		
		if (pthread_mutex_unlock((idConta < idConta2 ? &(trincos_contas[idConta - 1]) : &(trincos_contas[idConta2 - 1]))) != 0) {
			
			perror ("pthread_mutex_lock(trincos_contas)");
			exit(EXIT_FAILURE);
		}
		
		break;

	case OP_SAIR:
		
		pthread_exit(NULL);
		break;

	default:
		printf("Erro: valor %d desconhecido.\n", oper);
	}

	if (logFileDescriptor != -1) {

		if (write(logFileDescriptor, logBuffer, toWrite) == -1) {
			
			perror("write(logFileDescriptor,logBuffer, toWrite)");
			exit(EXIT_FAILURE);
		}
	}

	/* enviar resultado do comando recebido do terminal */
	if(bufSize > 0) {
		
		if ((fdE = open(fifoEscrita, O_WRONLY)) == -1) {
			
			perror("open(fifoEscrita)");
			exit(EXIT_FAILURE);
		} 
		
		if (write(fdE, buf, bufSize + 1) == -1) {

			perror("write(fdE, buf, bufSize)");
			close(fdE);
			exit(EXIT_FAILURE);
		}

		if (close(fdE) == -1) {

			perror("close(fdE)");
			exit(EXIT_FAILURE);
		}
	}
}