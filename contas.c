#include "contas.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

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

	int ano, i, aux, simulSaldo;
	
	for(ano = 0, ano <= numAnos, ano++) {
		printf("SIMULACAO: Ano %d\n", ano);
		printf("=================\n");

		for(i = 0, i < NUM_CONTAS, i++) {
			if(ano == 0)
				simulSaldo = contasSaldos[i];
			
			else {
				aux = (contasSaldos[i]*(1 + TAXAJURO) - CUSTOMANUTENCAO);
				simulSaldo = (aux > 0 ? aux : 0);
			}
			printf("Conta %d, Saldo %d\n", (i+1), simulSaldo );
		}
		printf("\n");
	}
}
