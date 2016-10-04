#include "contas.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#define atrasar() sleep(ATRASO)
		     
int contasSaldos[NUM_CONTAS];


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
  int pid, estado;

  pid = fork();

  if (pid == 0) { 
    int ano, i, aux, simulSaldo;
    int auxContasSaldos[NUM_CONTAS];

    printf("--------------Estou num processo filho---------------------\n");

    for (i = 0; i < NUM_CONTAS; i++) {
      auxContasSaldos[i] = contasSaldos[i];
    }

    for(ano = 0; ano <= numAnos; ano++) {

      atrasar();

      printf("SIMULACAO: Ano %d\n", ano);
      printf("=================\n");

      for(i = 0; i < NUM_CONTAS; i++) {

        if(ano == 0)
          simulSaldo = auxContasSaldos[i];

        else {
          aux = (auxContasSaldos[i]*(1 + TAXAJURO) - CUSTOMANUTENCAO);
          simulSaldo = (aux > 0 ? aux : 0);
          auxContasSaldos[i] = simulSaldo;
        }

        printf("Conta %d, Saldo %d\n", (i+1), simulSaldo);
      }

      printf("\n");
    }

    printf("--------------Estou num processo pai~NOVO---------------------\n");
    exit(0);
  }

  else
    printf("--------------Estou num processo pai---------------------\n");
    pid = wait(&estado);
}