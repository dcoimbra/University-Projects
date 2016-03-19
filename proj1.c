/* Bibliotecas de funcoes */

#include <stdio.h>
#include <string.h>

/* Definicoes de constantes */

#define IDLEN 4             /* Tamanho das strings na qual se representam os codigos de identificacao */
#define MAXAERO 1000        /* Numero maximo de aeroportos a criar e de voos em cada aeroporto.       */
#define ABERTO  1
#define FECHADO 0

/* Estruturas */

typedef struct
{
	char id[IDLEN];
	int capacidade, estado;       /* Aeroporto caraterizado por um um codigo de identificacao de 4 letras, */
} aeroporto;			      /* capacidade maxima de voos e um estado 0 (encerrado) ou 1 (aberto)     */

/* Prototipo */

void adicionaAeroporto(aeroporto vet_aeroportos[]);
void alteraCapacidadeMaxima();
void adicionaVooIdaVolta();
void adicionaRota();
void removeVooIda();
void removeVooIdaVolta();
void numeroVoos();
void aeroportoComMaisVoos();
void aeroportoMaisConectado();
void vooMaisPopular();
void encerraAeroporto();
void reabreAeroporto();
void emiteListagem(aeroporto vet_aeroportos[], int numeroAeroportos);

/* Programa principal */

int main()
{
	int numeroAeroportos = 0;
	char comando;
	aeroporto vet_aeroportos[MAXAERO];  /* Vetor que representa os aeroportos criados,
	                                       ordenados da esquerda para a direita pela
	                                       ordem que foram adicionados. */

	while (1)                          /* O programa pedira' sempre um comando, */
	{                                  /* exceto se algo correr mal ou o        */
	    comando = getchar();           /* utilizador terminar o programa.       */

		switch (comando)
		{
			case 'A':

				adicionaAeroporto(vet_aeroportos);

				numeroAeroportos++;

				break;

		 /*	case 'I':
				alteraCapacidadeMaxima();
				break;

			case 'F':
				adicionaVooIdaVolta();
				break;

			case 'G':
				adicionaRota();
				break;

			case 'R':
				removeVooIda();
				break;

			case 'S':
				removeVooIdaVolta();
				break;

			case 'N':
				numeroVoos();
				break;

			case 'P':
				aeroportoComMaisVoos();
				break;

			case 'Q':
				aeroportoMaisConectado();
				break;

			case 'V':
				vooMaisPopular();
				break;

			case 'C':
				encerraAeroporto();
				break;

			case 'O':
				reabreAeroporto();
				break;  */

			case 'L':
				emiteListagem(vet_aeroportos, numeroAeroportos);
				break;

			case 'X':
				return 0; /* Programa terminado com sucesso */

			default:
				printf("ERRO: Comando desconhecido\n");
		}

		getchar(); /* le o '\n' introduzido pelo utilizador */
	}

	return -1; /* se chegou aqui algo correu mal */
}

/* Comando A - Adicionamento de aeroportos. Junta o aeroporto correspondente ao
	       codigo de identificacao introduzido com a capacidade escolhida
               ao conjunto de aeroportos. */

void adicionaAeroporto (aeroporto vet_aeroportos[])
{
	static int i = 0;    /* indice do aeroporto adicionado no vetor de aeroportos */
	aeroporto aero;

	scanf("%s %d", aero.id, &aero.capacidade); /* operacoes de leitura do comando A: codigo de identificacao e capacidade maxima */

	aero.estado = ABERTO                       /* o aeroporto, quando criado, é automaticamente aberto */

	vet_aeroportos[i++] = aero;                /* o indice e' incrementado para que o proximo aeroporto seja associado a essa    */
}											   /*  posicao na proxima chamada da funcao  */


/* Comando L - Listagem de aeroportos (ordem de criação, alfabética, numero de voos) */

void emiteListagem (aeroporto vet_aeroportos[], int numeroAeroportos)
{
	int i, tipo;

	scanf("%d", &tipo);

	switch(tipo)
	{
		/* Ordenacao por ordem de criacao */
		case 0:

			for (i = 0; i < numeroAeroportos; i++)
			{
				printf("%s:%d\n", vet_aeroportos[i].id, vet_aeroportos[i].capacidade);
			}

			break;
	}
}
