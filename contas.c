#include "contas.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#define atrasar() sleep(ATRASO)
		     
int contasSaldos[NUM_CONTAS];
int recebeuSignal = 0;

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

void simular(int numAnos) {

  int ano, i, aux, simulSaldo;
  int auxContasSaldos[NUM_CONTAS];

  if (signal(SIGUSR1, tratarSignal) == SIG_ERR) /* Faz o handle do signal e previne */
    perror ("Erro.");                  /* caso haja erro */

  for (i = 0; i < NUM_CONTAS; i++) 
    auxContasSaldos[i] = lerSaldo(i + 1);

  for (ano = 0; ano <= numAnos; ano++) {
    
    atrasar();
    printf("SIMULACAO: Ano %d\n", ano);
    printf("=================\n");

    for(i = 0; i < NUM_CONTAS; i++) {
      
      if (ano == 0)
        simulSaldo = auxContasSaldos[i];

      else {
        aux = (auxContasSaldos[i]*(1 + TAXAJURO) - CUSTOMANUTENCAO);
        simulSaldo = (aux > 0 ? aux : 0);  /* se a simulacao resultar num valor negativo, e' apresentado o valor 0. */
        auxContasSaldos[i] = simulSaldo;
      }

      printf("Conta %d, Saldo %d\n", (i + 1), simulSaldo);
    }
    
    if (recebeuSignal) {
    	printf("Simulacao terminada por signal\n\n");
    	exit(EXIT_SUCCESS);
    }

    printf("\n");
  }

  exit(EXIT_SUCCESS); /* retorna ao processo pai */
}

void tratarSignal(int signum) {
  
  recebeuSignal = 1;
}
