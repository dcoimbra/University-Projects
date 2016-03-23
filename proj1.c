/* Bibliotecas de funcoes */

#include <stdio.h>
#include <string.h>

/* Definicoes de constantes */

#define IDLEN 4             /* Tamanho das strings na qual se representam os codigos de identificacao */
#define MAXAERO 1000        /* Numero maximo de aeroportos a criar e de voos em cada aeroporto.       */
#define ABERTO 1
#define ENCERRADO 0
#define TRUE 1

/* Estruturas */

typedef struct
{
	char id[IDLEN];
	int capacidade, estado;   /* Aeroporto caraterizado por um um codigo de identificacao de 4 letras, */
} aeroporto;			      /* capacidade maxima de voos e um estado 0 (encerrado) ou 1 (aberto)     */

/* Prototipo - programa principal */

void adicionaAeroporto(aeroporto vet_aeroportos[]);
void alteraCapacidadeMaxima(aeroporto vet_aeroportos[], int numeroAeroportos);
void adicionaVooIdaVolta();
void adicionaRota();
void removeVooIda();
void removeVooIdaVolta();
void numeroVoos();
void aeroportoComMaisVoos();
void aeroportoMaisConectado();
void vooMaisPopular();
void encerraAeroporto(aeroporto vet_aeroportos[], int numeroAeroportos);
void reabreAeroporto(aeroporto vet_aeroportos[], int numeroAeroportos);
void emiteListagem(aeroporto vet_aeroportos[], int numeroAeroportos);

/* Prototipo - funcoes auxiliares */

int indiceAeroporto(aeroporto vet_aeroportos[], int numeroAeroportos, char aero_id[]);
void printAeroportos (aeroporto vet_aeroportos[], int numeroAeroportos);

/* Programa principal */

int main()
{
	int numeroAeroportos = 0;
	char comando;
	aeroporto vet_aeroportos[MAXAERO];  /* Vetor que representa os aeroportos criados,
	                                       ordenados da esquerda para a direita pela
	                                       ordem que foram adicionados. */

	while (TRUE)                          /* O programa pedira' sempre um comando, */
	{                                  /* exceto se algo correr mal ou o        */
	    comando = getchar();           /* utilizador terminar o programa.       */

		switch (comando)
		{
			case 'A':

				adicionaAeroporto(vet_aeroportos);

				numeroAeroportos++;

				break;

		 	case 'I':

				alteraCapacidadeMaxima(vet_aeroportos, numeroAeroportos);

				break;

		/*	case 'F':
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
				break; */

			case 'C':
				encerraAeroporto(vet_aeroportos, numeroAeroportos);

				break;

			case 'O':

				reabreAeroporto(vet_aeroportos, numeroAeroportos);

				break;

			case 'L':

				emiteListagem (vet_aeroportos, numeroAeroportos);

				break;

			case 'X':

				printf(":%d\n", numeroAeroportos);
				
				return 0; /* Programa terminado com sucesso */
		}

		getchar(); /* le o '\n' introduzido pelo utilizador */
	}

	return -1; /* se chegou aqui algo correu mal */
}

/* Comando A - Adicao de aeroportos. Junta o aeroporto correspondente ao
	       codigo de identificacao introduzido com a capacidade escolhida
               ao conjunto de aeroportos. */

void adicionaAeroporto(aeroporto vet_aeroportos[])
{
	static int i = 0;
	aeroporto aero;

	scanf("%s %d", aero.id, &aero.capacidade); /* operacoes de leitura do comando A: codigo de identificacao e capacidade maxima */

	aero.estado = ABERTO;                       /* o aeroporto, quando criado, e' automaticamente aberto */

	vet_aeroportos[i++] = aero;                /* o indice e' incrementado para que o proximo aeroporto seja associado a essa    */
}											   /*  posicao na proxima chamada da funcao  */

/* Comando I - Altera a capacidade maxima do aeroporto correspondente ao ID introduzido */

void alteraCapacidadeMaxima(aeroporto vet_aeroportos[], int numeroAeroportos)
{
	int i, aumento_capacidade;
	char aero_id[IDLEN];

	scanf("%s %d", aero_id, &aumento_capacidade);

	i = indiceAeroporto(vet_aeroportos, numeroAeroportos, aero_id);

	if ((i != -1) && (vet_aeroportos[i].estado == ABERTO))
	{
		vet_aeroportos[i].capacidade += aumento_capacidade; /* A capacidade so e alterada caso o aeroporto exista e esteja aberto */ 
	}

	else
	{
		printf("*Capacidade de %s inalterada\n", aero_id);
	}
}

/* Comando C - muda o estado do aeroporto para ENCERRADO. */

void encerraAeroporto(aeroporto vet_aeroportos[], int numeroAeroportos)
{
	int i;
	char aero_id[IDLEN];

	scanf("%s", aero_id);

	i = indiceAeroporto(vet_aeroportos, numeroAeroportos, aero_id);

	if (i == -1)
	{
		printf("*Aeroporto %s inexistente\n", aero_id);
	}

	else
	{
		vet_aeroportos[i].estado = ENCERRADO;
	}
}

/* Comando O - muda o estado do aeroporto para ABERTO. */

void reabreAeroporto(aeroporto vet_aeroportos[], int numeroAeroportos)
{
	int i;
	char aero_id[IDLEN];

	scanf("%s", aero_id);

	i = indiceAeroporto(vet_aeroportos, numeroAeroportos, aero_id);

	if (i == -1)
	{
		printf("*Aeroporto %s inexistente\n", aero_id);
	}

	else
	{
		vet_aeroportos[i].estado = ABERTO;
	}
}

/* Comando L - Listagem de aeroportos (ordem de criacao, alfabetica, numero de voos) */

void emiteListagem(aeroporto vet_aeroportos[], int numeroAeroportos)
{
	int tipo;

	scanf("%d", &tipo);

	switch (tipo)
	{
		/* Ordenacao por ordem de criacao */
		case 0:

			printAeroportos(vet_aeroportos, numeroAeroportos);

			break;
	}
}

/* Funcoes auxiliares */

/* indiceAeroporto - verifica se o aeroporto existe e retorna o seu indice, ou -1 se nao existir */

int indiceAeroporto(aeroporto vet_aeroportos[], int numeroAeroportos, char aero_id[])
{
	int i;

	for (i = 0; i < numeroAeroportos; i++)
	{
		if (strcmp(aero_id, vet_aeroportos[i].id) == 0)
		{
			return i;
		}
	}

	return -1;    /* se chegou aqui o aeroporto nao existe */	
}

/* printAeroportos - percorre um conjuto de aeroportos e coloca no ecrã os seus códigos de identificação e atual capacidade. */

void printAeroportos (aeroporto vet_aeroportos[], int numeroAeroportos)
{
	int i;

	for (i = 0; i < numeroAeroportos; i++)
	{
		printf("%s:%d:\n", vet_aeroportos[i].id, vet_aeroportos[i].capacidade);
	}
}