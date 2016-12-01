#ifndef I_BANCO_H
#define I_BANCO_H




#define COMANDO_DEBITAR "debitar"
#define COMANDO_CREDITAR "creditar"
#define COMANDO_LER_SALDO "lerSaldo"
#define COMANDO_SIMULAR "simular"
#define COMANDO_TRANSFERIR "transferir"
#define COMANDO_SAIR "sair"
#define COMANDO_SAIR_AGORA "agora"
#define COMANDO_SAIR_TERMINAL "sair-terminal"

#define OP_DEBITAR 2
#define OP_CREDITAR 3
#define OP_LERSALDO 4
#define OP_TRANSFERIR 5
#define OP_SIMULAR 6
#define OP_SAIR 1
#define OP_SAIR_AGORA 0

#define MAXARGS 4
#define BUFFER_SIZE 100

#define MAX_CHILDREN 20

#define NUM_TRABALHADORAS 3
#define CMD_BUFFER_DIM (NUM_TRABALHADORAS * 2)

#define MAX_STR_SIZE 48
#define MAX_BUF 1024

typedef struct {
		int operacao;
		int idConta1;
		int idConta2;
		int valor;
		char fifoL[MAX_STR_SIZE];
} comando_t;

#endif