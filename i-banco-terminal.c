/*
// Projeto SO - exercicio 4, version 1
// Sistemas Operativos, DEI/IST/ULisboa 2016-17
*/

#include "i-banco.h"

void envia_trabalho(int oper, int accountID1, int accountID2, int moneyValue, int fdE, char* fifoL);

void funcSP() { }

int fdLeitura;


int main (int argc, char** argv) {

	char *args[MAXARGS + 1];
	char buffer[BUFFER_SIZE];

	char fifoLeitura[MAX_STR_SIZE];
	char *fifoEscrita;

	int fdEscrita;

	if (signal(SIGPIPE, funcSP) == SIG_ERR) {
		perror("signal(SIGPIPE)");
	}

	if(argc < 2) {
		printf("Erro: numero de argumentos insuficiente.");
		exit(EXIT_FAILURE);
	}

	fifoEscrita = argv[1];

	if ((fdEscrita = open(fifoEscrita, O_WRONLY)) == -1) {
		perror("open(fifo)");
	}

	snprintf(fifoLeitura, MAX_STR_SIZE, "/tmp/i-banco-pipe-%d", getpid());
    if(mkfifo(fifoLeitura, 0666) != 0) {
    	perror("mkfifo");
    }


	printf("Bem-vinda/o ao i-banco\n\n");
			
	while (1) {
		int numargs;
		printf(">> ");

		numargs = readLineArguments(args, MAXARGS+1, buffer, BUFFER_SIZE);

		/* EOF (end of file) do stdin ou comando "sair" */
		if (numargs < 0 ||
		   (numargs > 0 && (strcmp(args[0], COMANDO_SAIR) == 0))) {


			/* Sair Agora */
			if ((args[1] != NULL) && (strcmp(args[1], COMANDO_SAIR_AGORA)) == 0)
				envia_trabalho(OP_SAIR_AGORA, 0, 0, 0, fdEscrita, fifoLeitura);	

			else
				envia_trabalho(OP_SAIR, 0, 0, 0, fdEscrita, fifoLeitura);

		}

		else if (strcmp(args[0], COMANDO_SAIR_TERMINAL) == 0) {

			printf("i-banco-terminal vai terminar.\n--\n");

			close(fdEscrita);
			unlink(fifoLeitura);

			printf("\n--\ni-banco-terminal terminou.\n");
			exit(EXIT_SUCCESS);
		}
		
		else if (numargs == 0)			
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

			envia_trabalho(OP_DEBITAR, idConta, 0, valor, fdEscrita, fifoLeitura);
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

			envia_trabalho(OP_CREDITAR, idConta, 0, valor, fdEscrita, fifoLeitura);
		}

		/* Ler Saldo */
		else if (strcmp(args[0], COMANDO_LER_SALDO) == 0) {
			
			int idConta;

			if (numargs < 2) {
				
				printf("%s: Sintaxe inválida, tente de novo.\n", COMANDO_LER_SALDO);
				continue;
			}

			idConta = atoi(args[1]);

			envia_trabalho(OP_LERSALDO, idConta, 0, 0, fdEscrita, fifoLeitura);
		}

		/* Transferir */
		else if (strcmp(args[0], COMANDO_TRANSFERIR) == 0) {

			int idConta1, idConta2, valor;

			if (numargs < 4) {
				
				printf("%s: Sintaxe inválida, tente de novo.\n", COMANDO_TRANSFERIR);
				continue;
			}

			idConta1 = atoi(args[1]);
			idConta2 = atoi(args[2]);
			valor = atoi(args[3]);

			envia_trabalho(OP_TRANSFERIR, idConta1, idConta2, valor, fdEscrita, fifoLeitura);
		}
					
		/* Simular */
		else if (strcmp(args[0], COMANDO_SIMULAR) == 0) {

			int numAnos;

			if (numargs < 2) {

				printf("%s: Sintaxe inválida, tente de novo.\n", COMANDO_SIMULAR);
				continue;
			}
			
			numAnos = atoi(args[1]);

			if (numAnos < 0)
				printf("%s(%d): Erro.\n\n", COMANDO_SIMULAR, numAnos);
			
			else
				envia_trabalho(OP_SIMULAR, 0, 0, numAnos, fdEscrita, fifoLeitura);

		}

		else 
			printf("Comando desconhecido. Tente de novo.\n");
	} 
}

void envia_trabalho(int oper, int accountID1, int accountID2, int value, int fdEscrita, char* fifoLeitura) {
	
	time_t start_t, end_t;
	int fdLeitura;
	char buf[MAX_BUF];
	int  wrValue = 0;

	comando_t trabalho;
	trabalho.operacao = oper;
	trabalho.idConta1 = accountID1;
	trabalho.idConta2 = accountID2;
	trabalho.valor = value;
	strcpy(trabalho.fifoL, fifoLeitura);
	

	time(&start_t);
	if( (wrValue = write(fdEscrita, (void*)&trabalho, sizeof(trabalho))) == -1)  {
		printf("i-banco-pipe nao existe\n Por favor feche o terminal (sair-terminal)\n");
		return;
	}
	
	printf("wr value: %d\n", wrValue);

	if(oper == OP_SIMULAR || oper == OP_SAIR || oper == OP_SAIR_AGORA) {
		return;
	}

	printf("aguardando resposta\n");
	if((fdLeitura = open(fifoLeitura, O_RDONLY)) == -1) {
		perror("open(fifoLeitura)");
	}

	
	read(fdLeitura, buf, MAX_BUF);
	time(&end_t);
	printf("%s", buf);
	printf("tempo de execucao: %ds\n", (int)difftime(end_t, start_t));

	close(fdLeitura);

}


