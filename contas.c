#include "contas.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#define atrasar() sleep(ATRASO)
		     
int contasSaldos[NUM_CONTAS];
int recebeuSignal = 0; /* Flag. Indica se o processo recebeu um signal. */

int contaExiste(int idConta) {
  return (idConta > 0 && idConta <= NUM_CONTAS);
}

void inicializarContas() {
  int i;
  for (i=0; i<NUM_CONTAS; i++)
    contasSaldos[i] = 0;
}

int debitar(int idConta, int valor) {
  atrasar();
  if (!contaExiste(idConta))
    return -1;
  if (contasSaldos[idConta - 1] < valor)
    return -1;
  atrasar();
  contasSaldos[idConta - 1] -= valor;
  return 0;
}

int creditar(int idConta, int valor) {
  atrasar();
  if (!contaExiste(idConta))
    return -1;
  contasSaldos[idConta - 1] += valor;
  return 0;
}

int lerSaldo(int idConta) {
  atrasar(); 
  if (!contaExiste(idConta))
    return -1;
  return contasSaldos[idConta - 1];
}

int transferir(int idConta1, int idConta2, int valor) {
	atrasar();
	if (!contaExiste(idConta1) || !contaExiste(idConta2))
		return -1;
	contasSaldos[idConta1 - 1] -= valor;
	contasSaldos[idConta2 - 1] += valor;
	return 0;
}

void simular(int numAnos) {

  int ano, i, saldo;

  if (signal(SIGUSR1, tratarSignal) == SIG_ERR) /* Faz o handle do signal e indica*/
    perror ("Erro.");                           /* caso haja erro */

  for (ano = 0; ano <= numAnos; ano++) {
    
    printf("SIMULACAO: Ano %d\n", ano);
    printf("=================\n");

    for(i = 0; i < NUM_CONTAS; i++) {

      if (ano > 0) {

      	saldo = lerSaldo(i + 1);
        creditar((i + 1), saldo * TAXAJURO);
        
        saldo = lerSaldo(i + 1);
        debitar((i + 1), (CUSTOMANUTENCAO > saldo) ? saldo : CUSTOMANUTENCAO);
      }

      saldo = lerSaldo(i + 1);
      
      printf("Conta %d, Saldo %d\n", (i + 1), saldo);
    }

    if (recebeuSignal) {
    	printf("Simulacao terminada por signal\n\n");
    	exit(EXIT_SUCCESS);
    }

    printf("\n");
  }
}

/* Quando o processo recebe um signal, 
   o valor da flag é alterado para o refletir. */
void tratarSignal(int signum) {
  
  recebeuSignal = 1;
}
