/* Bibliotecas de funcoes */

#include <stdio.h>
#include <string.h>

/* Definicoes de constantes */

#define IDLEN 4             /* Tamanho das strings na qual se representam os codigos de identificacao */
#define MAXAERO 1000        /* Numero maximo de aeroportos a criar e de voos em cada aeroporto.       */
#define ABERTO 1
#define ENCERRADO 0
#define TRUE 1
#define FALSE 0

/* Estruturas */

typedef struct
{
	char id[IDLEN];
	int capacidade, estado;   /* Aeroporto caraterizado por um codigo de identificacao de 3 letras, */
	int incoming, outgoing;   /* capacidade maxima de voos e um estado: 0 (encerrado) ou 1 (aberto).*/
	} aeroporto;			  /* o n. de voos total que sai e chega a um aeroporto tambem esta associado ao mesmo. */


/* Prototipo - funcoes principais */

void adicionaAeroporto(aeroporto vet_aeroportos[]);
void alteraCapacidadeMaxima(aeroporto vet_aeroportos[], int numero_aeroportos);
void numeroVoos(aeroporto vet_aeroportos[], int numero_aeroportos, int matriz_voos[][MAXAERO]);
void aeroportoComMaisVoos(aeroporto vet_aeroportos[], int numero_aeroportos);
void aeroportoMaisConectado(aeroporto vet_aeroportos[], int numero_aeroportos, int matriz_voos[][MAXAERO]);
void vooMaisPopular(aeroporto vet_aeroportos[], int numero_aeroportos, int matriz_voos[][MAXAERO]);
void encerraAeroporto(aeroporto vet_aeroportos[], int numero_aeroportos, int matriz_voos[][MAXAERO]);
void reabreAeroporto(aeroporto vet_aeroportos[], int numero_aeroportos);
void emiteListagem(aeroporto vet_aeroportos[], int numero_aeroportos);

/* Prototipo - funcoes auxiliares */

void inicializaMatriz(int matriz_voos[MAXAERO][MAXAERO]);
int indiceAeroporto(aeroporto vet_aeroportos[], int numero_aeroportos, char aero_id[]);
void printAeroportos (aeroporto vet_aeroportos[], int numero_aeroportos);
void printAeroportoSorted(aeroporto vet_aeroportos[], int numero_aeroportos);
void printDistrVoosAeroportos(aeroporto vet_aeroportos[], int numero_aeroportos);
int totalVoos (aeroporto vet_aeroportos[], int numero_aeroportos);
void adicionaVoos(aeroporto vet_aeroportos[], int numero_aeroportos, int matriz_voos[][MAXAERO], int ida_volta);
void removeVoos(aeroporto vet_aeroportos[], int numero_aeroportos, int matriz_voos[][MAXAERO], int ida_volta);


/* Programa principal */

int main()
{
	int numero_aeroportos = 0;
	int matriz_voos[MAXAERO][MAXAERO];  /* Matriz de voos entre um aeroporto i para um aeroporto j */
	char comando;
	aeroporto vet_aeroportos[MAXAERO];  /* Vetor que representa os aeroportos criados,
									   ordenados da esquerda para a direita pela
									   ordem que foram adicionados. */
	inicializaMatriz(matriz_voos);

	while (TRUE)                          /* O programa pedira' sempre um comando, */
	{                                     /* exceto se algo correr mal ou o        */
	    comando = getchar();              /* utilizador terminar o programa.       */

		switch (comando)
		{
			case 'A':
				adicionaAeroporto(vet_aeroportos);
				numero_aeroportos++;
				break;

		 	case 'I':
				alteraCapacidadeMaxima(vet_aeroportos, numero_aeroportos);
				break;
				
			/* Comando F - cria viagem de ida e volta entre os dois aeroportos especificados */		
			case 'F':
				adicionaVoos(vet_aeroportos, numero_aeroportos, matriz_voos, TRUE);
				break;
				
			/* Comando G - cria viagem de ida entre os dois aeroportos especificados (do primeiro para o segundo) */
			case 'G':
				adicionaVoos(vet_aeroportos, numero_aeroportos, matriz_voos, FALSE);
				break;
				
			/* Comando R - remove viagem de ida entre os dois aeroportos especificados (do primeiro para o segundo) */		
			case 'R':
				removeVoos(vet_aeroportos, numero_aeroportos, matriz_voos, FALSE);
				break;
				
			/* Comando S - remove viagem de ida e volta entre os dois aeroportos especificados */		
			case 'S':
				removeVoos(vet_aeroportos, numero_aeroportos, matriz_voos, TRUE);
				break;

			case 'N':
				numeroVoos(vet_aeroportos, numero_aeroportos, matriz_voos);
				break;

			case 'P':
				aeroportoComMaisVoos(vet_aeroportos, numero_aeroportos);
				break;
		
			case 'Q':
				aeroportoMaisConectado(vet_aeroportos, numero_aeroportos, matriz_voos);
				break;

			case 'V':
				vooMaisPopular(vet_aeroportos, numero_aeroportos, matriz_voos);
				break; 

			case 'C':
				encerraAeroporto(vet_aeroportos, numero_aeroportos, matriz_voos);
				break;

			case 'O':
				reabreAeroporto(vet_aeroportos, numero_aeroportos);
				break;

			case 'L':
				emiteListagem (vet_aeroportos, numero_aeroportos);
				break;

			case 'X':
				printf("%d:%d\n", totalVoos(vet_aeroportos, numero_aeroportos), numero_aeroportos);
				return 0; /* Programa terminado com sucesso */
		}

		getchar(); /* le o '\n' introduzido pelo utilizador */
	}

	return -1; /* se chegou aqui algo correu mal */
}


/* Comando A - Cria o aeroporto correspondente ao codigo de identificacao introduzido e capacidade escolhida ao conjunto de aeroportos. */
void adicionaAeroporto(aeroporto vet_aeroportos[])
{
	static int i = 0;
	aeroporto aero;

	scanf("%s %d", aero.id, &aero.capacidade); /* operacoes de leitura do comando A: codigo de identificacao e capacidade maxima */

	aero.estado = ABERTO;                       /* o aeroporto, quando criado, e' automaticamente aberto */
	
	aero.incoming = 0;
	aero.outgoing = 0;

	vet_aeroportos[i++] = aero;                /* o indice e' incrementado para que o proximo aeroporto seja associado a essa    */
}											   /*  posicao na proxima chamada da funcao  */


/* Comando I - Altera a capacidade maxima do aeroporto correspondente ao ID introduzido */
void alteraCapacidadeMaxima(aeroporto vet_aeroportos[], int numero_aeroportos)
{
	int i, aumento_capacidade;
	char aero_id[IDLEN];

	scanf("%s %d", aero_id, &aumento_capacidade);

	i = indiceAeroporto(vet_aeroportos, numero_aeroportos, aero_id);
	
	if (i != -1) 
	{
		int voos_aeroporto = vet_aeroportos[i].incoming + vet_aeroportos[i].outgoing;
		int nova_capacidade = vet_aeroportos[i].capacidade + aumento_capacidade;
		
		if ((vet_aeroportos[i].estado == ABERTO) && (nova_capacidade >= voos_aeroporto)  && (nova_capacidade > 0))
		{
			vet_aeroportos[i].capacidade = nova_capacidade; /* A capacidade so e alterada caso o aeroporto exista, esteja aberto */ 
			return;											/* e caso a nova capacidade seja maior ou igual ao numero de voos do mesmo */
		}
	}
	printf("*Capacidade de %s inalterada\n", aero_id);
}


/* Comando N - indica o numero de voos entre dois aeroportos. */
void numeroVoos(aeroporto vet_aeroportos[], int numero_aeroportos, int matriz_voos[][MAXAERO])
{
	int i, j;
	char aero_id1[IDLEN], aero_id2[IDLEN];

	scanf("%s %s", aero_id1, aero_id2);

	i = indiceAeroporto(vet_aeroportos, numero_aeroportos, aero_id1);
	j = indiceAeroporto(vet_aeroportos, numero_aeroportos, aero_id2);

	if ((i != -1) && (j != -1))
	{
		printf("Voos entre cidades %s:%s:%d:%d\n", aero_id1, aero_id2, matriz_voos[i][j], matriz_voos[j][i]);
		return;
	}
	
	if (i == -1)
	{
		printf("*Aeroporto %s inexistente\n", aero_id1);
	}

	if (j == -1)
	{
		printf("*Aeroporto %s inexistente\n", aero_id2);
	}
}


/* Comando C - muda o estado do aeroporto para ENCERRADO. */
void encerraAeroporto(aeroporto vet_aeroportos[], int numero_aeroportos, int matriz_voos[][MAXAERO])
{
	int i, j;
	char aero_id[IDLEN];

	scanf("%s", aero_id);

	i = indiceAeroporto(vet_aeroportos, numero_aeroportos, aero_id);

	if (i == -1)
	{
		printf("*Aeroporto %s inexistente\n", aero_id);
	}

	else
	{
		vet_aeroportos[i].estado = ENCERRADO;
		vet_aeroportos[i].incoming = 0;
		vet_aeroportos[i].outgoing = 0;
		
		for(j = 0; j < numero_aeroportos; j++)
		{
			vet_aeroportos[j].incoming -= matriz_voos[i][j];
			matriz_voos[i][j] = 0;

			vet_aeroportos[j].outgoing -= matriz_voos[j][i];
			matriz_voos[j][i] = 0;
		}
	}
}


/* Comando O - muda o estado do aeroporto para ABERTO. */
void reabreAeroporto(aeroporto vet_aeroportos[], int numero_aeroportos)
{
	int i;
	char aero_id[IDLEN];

	scanf("%s", aero_id);

	i = indiceAeroporto(vet_aeroportos, numero_aeroportos, aero_id);

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
void emiteListagem(aeroporto vet_aeroportos[], int numero_aeroportos)
{
	int tipo;

	scanf("%d", &tipo);

	switch (tipo)
	{
		/* Ordenacao por ordem de criacao */
		case 0:
			printAeroportos(vet_aeroportos, numero_aeroportos);
			break;
		
		/* Ordenacao por ordem alfabetica */
		case 1:
			printAeroportoSorted(vet_aeroportos, numero_aeroportos);
			break;
		
		/* n:aeroportos com n voos - por ordem crescente de n (numero de voos) */
		case 2:
			printDistrVoosAeroportos(vet_aeroportos, numero_aeroportos);
			break;
	}
}


/* Comando P - devolve o aeroporto com o maior numero de voos */
void aeroportoComMaisVoos(aeroporto vet_aeroportos[], int numero_aeroportos)
{
	int i, maior_num_voos = 0, m_indice = 0;
	
	for (i = 0; i < numero_aeroportos; i++)
	{
		int num_voos = vet_aeroportos[i].incoming + vet_aeroportos[i].outgoing;
		
		if (num_voos > maior_num_voos)
		{
			maior_num_voos = num_voos;
			m_indice = i;
		}
	}
	printf("Aeroporto com mais rotas %s:%d:%d\n", vet_aeroportos[m_indice].id, vet_aeroportos[m_indice].outgoing, vet_aeroportos[m_indice].incoming);
}

/* Comando Q - devolve o aeroporto com mais aeroportos conectados */
void aeroportoMaisConectado(aeroporto vet_aeroportos[], int numero_aeroportos, int matriz_voos[][MAXAERO])
{
	int i, j, max_ligacoes_aeroportos = 0, indice = 0;

	for (i = 0; i < numero_aeroportos; i++)
	{
		int ligacoes_aeroportos = 0;
		for (j = 0; j < numero_aeroportos; j++)
		{
			if (matriz_voos[i][j] != 0)
			{
				ligacoes_aeroportos += 1;
			}
			else if (matriz_voos[j][i] != 0)
			{
				ligacoes_aeroportos += 1;
			}
		}
		
		if (ligacoes_aeroportos > max_ligacoes_aeroportos)
		{
			max_ligacoes_aeroportos = ligacoes_aeroportos;
			indice = i;
		} 
	}
	
	printf("Aeroporto com mais ligacoes %s:%d\n", vet_aeroportos[indice].id, max_ligacoes_aeroportos);
}


/* Comando V - imprime para o ecra quais os dois aeroportos com o maior numero de voos */
void vooMaisPopular(aeroporto vet_aeroportos[], int numero_aeroportos, int matriz_voos[][MAXAERO])
{
	int i, j, indice1 = 0, indice2 = 0, max_voos = 0;
	
	for (i = 0; i < numero_aeroportos; i++)
	{
		for (j = 0; j < numero_aeroportos; j++)
		{
			if (matriz_voos[i][j] > max_voos)
			{
				max_voos = matriz_voos[i][j];
				indice1 = i;
				indice2 = j;
			}
		}
	}
	printf("Voo mais popular %s:%s:%d\n", vet_aeroportos[indice1].id, vet_aeroportos[indice2].id, max_voos);
}

/* Funcoes auxiliares */

/* inicializaMatriz - inicializa todos os valores da matriz de voos a 0 */
void inicializaMatriz(int matriz_voos[MAXAERO][MAXAERO])
{
	int x, y;
	for(x = 0; x < MAXAERO; x ++) 
	{
		for(y = 0; y < MAXAERO; y ++) 
			matriz_voos[x][y] = 0;
	}
}

/* indiceAeroporto - verifica se o aeroporto existe e retorna o seu indice, ou -1 se nao existir */
int indiceAeroporto(aeroporto vet_aeroportos[], int numero_aeroportos, char aero_id[])
{
	int i;

	for (i = 0; i < numero_aeroportos; i++)
	{
		if (strcmp(aero_id, vet_aeroportos[i].id) == 0)
		{
			return i;
		}
	}

	return -1;    /* se chegou aqui o aeroporto nao existe */	
}


/* printAeroportos - coloca no ecra o numero de voos de ida e de volta, mais a capacidade e id de todos os aeroportos na rede. */
void printAeroportos (aeroporto vet_aeroportos[], int numero_aeroportos)
{
	int i;

	for (i = 0; i < numero_aeroportos; i++)
	{
		printf("%s:%d:%d:%d\n", vet_aeroportos[i].id, vet_aeroportos[i].capacidade, vet_aeroportos[i].outgoing, vet_aeroportos[i].incoming);
	}
}


/* printAeroportoSorted - coloca no ecra o numero de voos de ida e de volta, mais a capacidade e id de todos os aeroportos na rede, por ordem alfabetica */
void printAeroportoSorted(aeroporto vet_aeroportos[], int numero_aeroportos)
{
	int i, j;
	int vet_index_ids[MAXAERO];
	
	for (i = 0; i < numero_aeroportos; i++)
	{
		vet_index_ids[i] = i;
	}
	
	for (i = 0; i < numero_aeroportos - 1; i++)
	{
		int aux,min = i;
		
		for (j = i+1; j <= numero_aeroportos - 1; j++)
		{
			if (strcmp(vet_aeroportos[vet_index_ids[j]].id, vet_aeroportos[vet_index_ids[min]].id) < 0)
				min = j;
		}
		
		aux = vet_index_ids[i]; 
		vet_index_ids[i] = vet_index_ids[min]; 
		vet_index_ids[min] = aux;
	}
	for (i = 0; i < numero_aeroportos; i++)
	{
		printf("%s:%d:%d:%d\n", vet_aeroportos[vet_index_ids[i]].id, vet_aeroportos[vet_index_ids[i]].capacidade, vet_aeroportos[vet_index_ids[i]].outgoing, vet_aeroportos[vet_index_ids[i]].incoming);
	}
}


/* printDistrVoosAeroportos - coloca no ecra quantos aeroportos tem n voos, por ordem de n crescente. */
void printDistrVoosAeroportos(aeroporto vet_aeroportos[], int numero_aeroportos)
{
	int vet_resultados[MAXAERO], vet_voos[MAXAERO], i;
	
	for (i = 0; i < MAXAERO; i++)
	{
		vet_resultados[i] = 0;
	}
	
	for (i = 0; i < numero_aeroportos; i++)
	{
		vet_voos[i] = vet_aeroportos[i].incoming + vet_aeroportos[i].outgoing;
		
		vet_resultados[ vet_voos[i] ] += 1;
	}
	
	for (i = 0; i < MAXAERO; i++)
	{
		if (vet_resultados[i] != 0)
		{
			printf("%d:%d\n", i, vet_resultados[i]);
		}
	}
}


/* totalVoos - devolve o numero total de voos existente na rede de aeroportos */
int totalVoos (aeroporto vet_aeroportos[], int numero_aeroportos)
{
	int i, total_voos = 0;
	
	for (i = 0; i < numero_aeroportos; i++)
	{
		total_voos += vet_aeroportos[i].outgoing;
	}
	
	return total_voos;
}


/* adicionaVoos - cria uma viagem de ida ou de ida e volta entre dois aeroportos, dependendo do valor do inteiro "ida_volta" */
void adicionaVoos(aeroporto vet_aeroportos[], int numero_aeroportos, int matriz_voos[][MAXAERO], int ida_volta)
{
	int i, j, num_voos, voos_aero1, voos_aero2;
	char aero_id1[IDLEN], aero_id2[IDLEN];
	
	scanf("%s %s", aero_id1, aero_id2);
	
	i = indiceAeroporto(vet_aeroportos, numero_aeroportos, aero_id1);
	j = indiceAeroporto(vet_aeroportos, numero_aeroportos, aero_id2);
	
	if (i != -1 && j != -1 )
	{
		voos_aero1 = vet_aeroportos[i].incoming + vet_aeroportos[i].outgoing;
		voos_aero2 = vet_aeroportos[j].incoming + vet_aeroportos[j].outgoing;
		
		if (vet_aeroportos[i].estado == ABERTO && vet_aeroportos[j].estado == ABERTO)
		{
			num_voos = ida_volta ? 2 : 1;
			
			if ((voos_aero1 + num_voos <= vet_aeroportos[i].capacidade) && (voos_aero2 + num_voos <= vet_aeroportos[j].capacidade))
			{
				matriz_voos[i][j] += 1;				
				vet_aeroportos[i].outgoing += 1;
				vet_aeroportos[j].incoming += 1;
				
				if(ida_volta)
				{
					matriz_voos[j][i] += 1;
					vet_aeroportos[j].outgoing += 1;
					vet_aeroportos[i].incoming += 1;					
				}
				
				return;
			}
		}
	}
	
	printf("*Impossivel adicionar voo %s%s %s\n", (ida_volta ? "RT " : ""), aero_id1, aero_id2);
} 


/* removeVoos - remove uma viagem de ida ou de ida e volta entre dois aeroportos, dependendo do valor do inteiro "ida_volta" */
void removeVoos(aeroporto vet_aeroportos[], int numero_aeroportos, int matriz_voos[][MAXAERO], int ida_volta)
{
	int i, j, verifica_volta, aero1_aero2, aero2_aero1;
	char aero_id1[IDLEN], aero_id2[IDLEN];
	
	scanf("%s %s", aero_id1, aero_id2);
	
	i = indiceAeroporto(vet_aeroportos, numero_aeroportos, aero_id1);
	j = indiceAeroporto(vet_aeroportos, numero_aeroportos, aero_id2);
	
	if (i != -1 && j != -1 )
	{
		if (vet_aeroportos[i].estado == ABERTO && vet_aeroportos[j].estado == ABERTO)
		{
			aero1_aero2 = matriz_voos[i][j];
			aero2_aero1 = matriz_voos[j][i];
		
			verifica_volta = ida_volta ? aero2_aero1 : 1;
			
			if (verifica_volta > 0 && aero1_aero2 > 0)
			{
				matriz_voos[i][j] -= 1;				
				vet_aeroportos[i].outgoing -= 1;
				vet_aeroportos[j].incoming -= 1;
			
				if (ida_volta)
				{
					matriz_voos[j][i] -= 1;
					vet_aeroportos[j].outgoing -= 1;
					vet_aeroportos[i].incoming -= 1;
				}
				
				return;	
			}			
		}
	}
	printf("*Impossivel remover voo %s%s %s\n", (ida_volta ? "RT " : ""), aero_id1, aero_id2);
}