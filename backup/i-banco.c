/*
// Projeto SO - exercicio 4, version 1
// Sistemas Operativos, DEI/IST/ULisboa 2016-17
*/

#include "i-banco.h"

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
	int nFilhos = 0;  
	int i, fdLeitura;
	char* fifoLeitura = "/tmp/i-banco-pipe";
	char simFileName[MAX_STR_SIZE];
	int operacao;
	int numAnos;
	pid_t pid;

	pthread_mutex_init(&(trinco_write), NULL);
	pthread_mutex_init(&(trinco_read), NULL);

	sem_init(&sem_write, 0, CMD_BUFFER_DIM);
	sem_init(&sem_read, 0, 0);

	for (i = 0; i < NUM_CONTAS; ++i) {
		pthread_mutex_init(&(trincos_contas[i]), NULL);
	}

	pthread_mutex_init(&count_mutex, NULL);
	pthread_cond_init(&count_cond, NULL);

	if (signal(SIGUSR1, tratarSignal) == SIG_ERR) /* Faz o handle do signal e indica*/
    	perror ("Signal(SIGUSR1");                           /* caso haja erro */

	logFileDescriptor = open("log.txt", O_CREAT | O_WRONLY | O_APPEND, S_IRWXU | S_IRWXG | S_IRWXO);
	if(logFileDescriptor == -1) {
		perror("open(logFileDescriptor)");
	}
	
	criaPoolTarefas();

	inicializarContas();


	if(mkfifo(fifoLeitura, 0666) != 0) {
		perror("mkfifo");
	}

	if((fdLeitura = open(fifoLeitura, O_RDONLY)) == -1) {
		perror("open(fifoLeitura");
	}
			
	while (1) {

		comando_t trabalho;
			
		int sizeoft = read(fdLeitura, &trabalho, sizeof(trabalho));
	    if(sizeoft == 0) {
	    	if(close(fdLeitura) == -1) {
	    	 perror("close");
	    	}
	    	
	    	printf("0 bytes: fechei fifo e vou voltar a abrir...\n...\n");
	    	fdLeitura = open(fifoLeitura, O_RDONLY);
	    	printf("abri o fifo outra vez!!\n\n");
	    	continue;
	    }


	    operacao = trabalho.operacao;

		switch(operacao) {
		
		case OP_SAIR_AGORA:

			enviarSignal(pidFilhos, nFilhos);
			trabalho.operacao = OP_SAIR;

		case OP_SAIR:

			printf("i-banco vai terminar.\n--\n");

			for (i = 0; i < NUM_TRABALHADORAS; i++) 
				cria_trabalho(trabalho);
			
			for (i = 0; i < NUM_TRABALHADORAS; i++)
				pthread_join((tid[i]), NULL);
			
			unlink(fifoLeitura);
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
			
			pthread_mutex_lock(&count_mutex);

			while(count != 0) {
				pthread_cond_wait(&count_cond, &count_mutex);
			}

			pid = fork();

			if (pid == 0) {

				int sim_fd;
				snprintf(simFileName, MAX_STR_SIZE, "i-banco-sim-%d.txt", getpid() );

				sim_fd = open(simFileName, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

				if(sim_fd == -1) {
					perror("open failed");
				}

				printf("vou duplicar o STDOUT para o ficheiro...\n");
				dup2(sim_fd, STDOUT_FILENO);


				simular(numAnos);


				if(close(sim_fd) == -1) {
					perror("close (fd) failed");
				}
				
				exit(EXIT_SUCCESS);
			}

			else {
				pidFilhos[nFilhos++] = pid;
			}
			
			pthread_mutex_unlock(&count_mutex);
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
		
		pid = wait(&estado);
					
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
	a ser posteriormente executada. Funciona como um produtor. */
void cria_trabalho(comando_t trabalho) {

	sem_wait(&sem_write);

	pthread_mutex_lock(&trinco_write);

	cmd_buffer[buff_write_idx] = trabalho;
	buff_write_idx = (buff_write_idx + 1) % CMD_BUFFER_DIM;


	pthread_mutex_lock(&count_mutex);
	count++;
	pthread_mutex_unlock(&count_mutex);


	pthread_mutex_unlock(&trinco_write); 
	
	sem_post(&sem_read);
}

/*Le os comandos colocados no buffer e realiza as operacoes associadas (via realiza_trabalho). Funciona como um consumidor.  */
void* tarefa_trabalhadora(void *dummy) {

	while(1) {

		comando_t trabalho;
		
		sem_wait(&sem_read);

	
		pthread_mutex_lock(&trinco_read);

		trabalho = cmd_buffer[buff_read_idx];
		buff_read_idx = (buff_read_idx + 1) % CMD_BUFFER_DIM;

		pthread_mutex_unlock(&trinco_read); 
	
		sem_post(&sem_write);

		realiza_trabalho(trabalho);

		pthread_mutex_lock(&count_mutex);
		count--;
		pthread_cond_signal(&count_cond);
		pthread_mutex_unlock(&count_mutex);
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
	char* fifoEscrita = trabalho.fifoL;
	int saldo;
	char buf[MAX_BUF];
	int bufSize=-1;
	int fdE;
	char logBuffer[MAX_STR_SIZE];
	int toWrite;
	pthread_t tid = pthread_self();

	switch(oper) {

		case OP_CREDITAR:
			pthread_mutex_lock(&(trincos_contas[idConta - 1]));

			if (creditar (idConta, valor) < 0)
				bufSize = snprintf(buf, MAX_BUF, "%s(%d, %d): Erro\n\n", COMANDO_CREDITAR, idConta, valor);
			else
				bufSize = snprintf(buf, MAX_BUF, "%s(%d, %d): OK\n\n", COMANDO_CREDITAR, idConta, valor);

			toWrite = snprintf(logBuffer, MAX_STR_SIZE, "%lu: %s %d %d\n", tid, COMANDO_CREDITAR, trabalho.idConta1, trabalho.valor);
			pthread_mutex_unlock(&(trincos_contas[idConta - 1]));
			break;

		case OP_DEBITAR:
			pthread_mutex_lock(&(trincos_contas[idConta - 1]));

			if (debitar (idConta, valor) < 0)
				bufSize = snprintf(buf, MAX_BUF, "%s(%d, %d): Erro\n\n", COMANDO_DEBITAR, idConta, valor);
			else
				bufSize = snprintf(buf, MAX_BUF, "%s(%d, %d): OK\n\n", COMANDO_DEBITAR, idConta, valor);

			toWrite = snprintf(logBuffer, MAX_STR_SIZE, "%lu: %s %d %d\n", tid, COMANDO_DEBITAR, trabalho.idConta1, trabalho.valor);
			pthread_mutex_unlock(&(trincos_contas[idConta - 1]));
			break;

		case OP_LERSALDO:
			pthread_mutex_lock(&(trincos_contas[idConta - 1]));

			saldo = lerSaldo (idConta);
			if (saldo < 0)
				bufSize = snprintf(buf, MAX_BUF, "%s(%d): Erro.\n\n", COMANDO_LER_SALDO, idConta);
			else 
				bufSize = snprintf(buf, MAX_BUF, "%s(%d): O saldo da conta é %d.\n\n", COMANDO_LER_SALDO, idConta, saldo);

			toWrite = snprintf(logBuffer, MAX_STR_SIZE, "%lu: %s %d\n", tid, COMANDO_LER_SALDO, trabalho.idConta1);
			pthread_mutex_unlock(&(trincos_contas[idConta-1]));
			break;

		case OP_TRANSFERIR:
			pthread_mutex_lock((idConta < idConta2 ? &(trincos_contas[idConta - 1]) : &(trincos_contas[idConta2 - 1])));
			pthread_mutex_lock((idConta > idConta2 ? &(trincos_contas[idConta - 1]) : &(trincos_contas[idConta2 - 1])));

			if (transferir (idConta, idConta2, valor) < 0)
				bufSize = snprintf(buf, MAX_BUF, "Erro ao %s valor da conta %d para a conta %d.\n\n", COMANDO_TRANSFERIR, idConta, idConta2);
			else
				bufSize = snprintf(buf, MAX_BUF, "%s(%d, %d, %d): OK\n\n", COMANDO_TRANSFERIR, idConta, idConta2, valor);

			toWrite = snprintf(logBuffer, MAX_STR_SIZE, "%lu: %s %d %d %d\n", tid, COMANDO_TRANSFERIR, trabalho.idConta1, trabalho.idConta2, trabalho.valor);
			pthread_mutex_unlock((idConta > idConta2 ? &(trincos_contas[idConta - 1]) : &(trincos_contas[idConta2 - 1])));
			pthread_mutex_unlock((idConta < idConta2 ? &(trincos_contas[idConta - 1]) : &(trincos_contas[idConta2 - 1])));
			break;


		case OP_SAIR:
			pthread_exit(NULL);
			break;

		default:
			printf("Erro: valor %d desconhecido.\n", oper);
	}

	if (logFileDescriptor != -1) {
		write(logFileDescriptor, logBuffer, toWrite);
	}
	

	if(bufSize > 0) {
		if ((fdE = open(fifoEscrita, O_WRONLY))== -1) {
			perror("open(fifoEscrita");
		} 
		
		write(fdE, buf, bufSize+1);
		close(fdE);
	}
}