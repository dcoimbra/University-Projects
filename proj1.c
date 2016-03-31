/* tg025 - Filipa Marques n.57842 e David Coimbra n.84708 */


/* Bibliotecas de funcoes */

#include <stdio.h>
#include <string.h>

/* Definicoes de constantes */

#define IDLEN 4             /* Tamanho das strings na qual se representam os codigos de identificacao. */
#define MAXAERO 1000        /* Numero maximo de aeroportos a criar. */
#define MAXVOOS 1001		/* Tamanho do vector que define o numero possivel de voos por aeroporto (entre 0 e 1000 voos). */
#define ABERTO 1
#define ENCERRADO 0
#define TRUE 1
#define FALSE 0
#define INEXISTENTE -1

/* Estruturas */

typedef struct
{
	char id[IDLEN];			  /* Aeroporto caraterizado por um codigo de identificacao de 3 letras, */
	int capacidade, estado;   /* capacidade maxima de voos e um estado: 0 (encerrado) ou 1 (aberto).*/
	int incoming, outgoing;   /* o n. de voos total que sai e chega a um aeroporto tambem esta associado ao mesmo. */
	} aeroporto;			


/* Prototipo - funcoes principais */

void adicionaAeroporto(aeroporto vet_aeroportos[], int numero_aeroportos);
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
			/* Comando A - Cria o aeroporto correspondente ao codigo de identificacao introduzido e capacidade escolhida ao conjunto de aeroportos. */
			case 'A':
				adicionaAeroporto(vet_aeroportos, numero_aeroportos);
				numero_aeroportos++;
				break;

			/* Comando I - Altera a capacidade maxima do aeroporto correspondente ao ID introduzido */
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

			/* Comando N - indica o numero de voos entre dois aeroportos. */
			case 'N':
				numeroVoos(vet_aeroportos, numero_aeroportos, matriz_voos);
				break;

			/* Comando P - devolve o aeroporto com o maior numero de voos */
			case 'P':
				aeroportoComMaisVoos(vet_aeroportos, numero_aeroportos);
				break;

			/* Comando Q - devolve o aeroporto com mais aeroportos conectados */
			case 'Q':
				aeroportoMaisConectado(vet_aeroportos, numero_aeroportos, matriz_voos);
				break;

			/* Comando V - imprime para o ecra quais os dois aeroportos com o maior numero de voos */
			case 'V':
				vooMaisPopular(vet_aeroportos, numero_aeroportos, matriz_voos);
				break; 

			/* Comando C - muda o estado do aeroporto para ENCERRADO. */
			case 'C':
				encerraAeroporto(vet_aeroportos, numero_aeroportos, matriz_voos);
				break;

			/* Comando O - muda o estado do aeroporto para ABERTO. */
			case 'O':
				reabreAeroporto(vet_aeroportos, numero_aeroportos);
				break;

			/* Comando L - Listagem de aeroportos (ordem de criacao, alfabetica, numero de voos) */
			case 'L':
				emiteListagem (vet_aeroportos, numero_aeroportos);
				break;

			/* Comando X - termina o programa. */
			case 'X':
				printf("%d:%d\n", totalVoos(vet_aeroportos, numero_aeroportos), numero_aeroportos);
				return 0; /* Programa terminado com sucesso */
		}

		getchar(); /* le o '\n' introduzido pelo utilizador */
	}

	return -1; /* se chegou aqui algo correu mal */
}

/* adicionaAeroporto - pede ao utilizador um ID e uma capacidade e cria um aeroporto com esses valores. */
void adicionaAeroporto(aeroporto vet_aeroportos[], int numero_aeroportos)
{
	aeroporto aero;

	scanf("%s %d", aero.id, &aero.capacidade);  /* operacoes de leitura do comando A: codigo de identificacao e capacidade maxima */

	aero.estado = ABERTO;                       /* o aeroporto, quando criado, e' automaticamente aberto */
	
	aero.incoming = 0;
	aero.outgoing = 0;

	vet_aeroportos[numero_aeroportos] = aero;
}

/* alteraCapacidadeMaxima - pede ao utilizador um ID e um valor correspondente ao aumento ou diminuicao da capacidade do aeroporto correspondente. */
void alteraCapacidadeMaxima(aeroporto vet_aeroportos[], int numero_aeroportos)
{
	int i, aumento_capacidade;
	char aero_id[IDLEN];

	scanf("%s %d", aero_id, &aumento_capacidade);

	i = indiceAeroporto(vet_aeroportos, numero_aeroportos, aero_id);
	
	if (i != INEXISTENTE) 
	{
		int voos_aeroporto = vet_aeroportos[i].incoming + vet_aeroportos[i].outgoing;
		int nova_capacidade = vet_aeroportos[i].capacidade + aumento_capacidade;
		
		if ((vet_aeroportos[i].estado == ABERTO) && (nova_capacidade >= voos_aeroporto) && (nova_capacidade > 0))
		{
			vet_aeroportos[i].capacidade = nova_capacidade; /* A capacidade so e alterada caso o aeroporto exista, esteja aberto */ 
			return;											/* e caso a nova capacidade seja maior ou igual ao numero de voos do mesmo */
		}
	}
	printf("*Capacidade de %s inalterada\n", aero_id);
}


/* numeroVoos - pede ao utilizador dois IDs e coloca no ecra o numero de voos entre os dois aeroportos correspondentes. */
void numeroVoos(aeroporto vet_aeroportos[], int numero_aeroportos, int matriz_voos[][MAXAERO])
{
	int i, j;
	char aero_id1[IDLEN], aero_id2[IDLEN];

	scanf("%s %s", aero_id1, aero_id2);

	i = indiceAeroporto(vet_aeroportos, numero_aeroportos, aero_id1);
	j = indiceAeroporto(vet_aeroportos, numero_aeroportos, aero_id2);

	if ((i != INEXISTENTE) && (j != INEXISTENTE))
	{
		printf("Voos entre cidades %s:%s:%d:%d\n", aero_id1, aero_id2, matriz_voos[i][j], matriz_voos[j][i]);
		return;
	}
	
	if (i == INEXISTENTE)
	{
		printf("*Aeroporto %s inexistente\n", aero_id1);
	}

	if (j == INEXISTENTE)
	{
		printf("*Aeroporto %s inexistente\n", aero_id2);
	}
}

/* aeroportoComMaisVoos - percorre o vetor de aeroportos e coloca no ecra o aeroporto com maior numero de voos (idas e voltas) */
void aeroportoComMaisVoos(aeroporto vet_aeroportos[], int numero_aeroportos)
{
	int i, maior_num_voos = 0, m_indice = 0;
	
	for (i = 0; i < numero_aeroportos; i++)
	{
		int num_voos = vet_aeroportos[i].incoming + vet_aeroportos[i].outgoing;  /* soma os voos de ida e volta do aeroporto i */
		
		if (num_voos > maior_num_voos)
		{
			maior_num_voos = num_voos;
			m_indice = i;
		}
	}
	printf("Aeroporto com mais rotas %s:%d:%d\n", vet_aeroportos[m_indice].id, vet_aeroportos[m_indice].outgoing, vet_aeroportos[m_indice].incoming);
}

/* aeroportoMaisConectado - coloca no ecra o aeroporto com mais ligacoes a outros aeroportos */
void aeroportoMaisConectado(aeroporto vet_aeroportos[], int numero_aeroportos, int matriz_voos[][MAXAERO])
{
	int i, j, max_ligacoes_aeroportos = 0, indice = 0;

	for (i = 0; i < numero_aeroportos; i++)
	{
		int ligacoes_aeroportos = 0;
		for (j = 0; j < numero_aeroportos; j++)
		{
			if (matriz_voos[i][j] != 0 || matriz_voos[j][i] != 0) /* se as entradas (i, j) e (j, i) forem 0, nao existe ligacao entre os aeroportos i e j */
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


/* vooMaisPopular - coloca no ecra o valor maximo da matriz de voos, correspondente ao voo mais popular */
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

/* encerraAeroporto - pede ao utilizador um ID e, caso exista, encerra o aeroporto correspondente */
void encerraAeroporto(aeroporto vet_aeroportos[], int numero_aeroportos, int matriz_voos[][MAXAERO])
{
	int i, j;
	char aero_id[IDLEN];

	scanf("%s", aero_id);

	i = indiceAeroporto(vet_aeroportos, numero_aeroportos, aero_id);

	if (i == INEXISTENTE)
	{
		printf("*Aeroporto %s inexistente\n", aero_id);
	}

	else
	{
		vet_aeroportos[i].estado = ENCERRADO;
		vet_aeroportos[i].incoming = 0;
		vet_aeroportos[i].outgoing = 0;
		
		for(j = 0; j < numero_aeroportos; j++)               /* retira as ligacoes entre o aeroporto encerrado e os restantes */
		{
			vet_aeroportos[j].incoming -= matriz_voos[i][j];   
			matriz_voos[i][j] = 0;

			vet_aeroportos[j].outgoing -= matriz_voos[j][i];
			matriz_voos[j][i] = 0;
		}
	}
}

/* reabreAeroporto - pede ao utilizador um ID e, caso exista, reabre o aeroporto correspondente. */
void reabreAeroporto(aeroporto vet_aeroportos[], int numero_aeroportos)
{
	int i;
	char aero_id[IDLEN];

	scanf("%s", aero_id);

	i = indiceAeroporto(vet_aeroportos, numero_aeroportos, aero_id);

	if (i == INEXISTENTE)
	{
		printf("*Aeroporto %s inexistente\n", aero_id);
	}

	else
	{
		vet_aeroportos[i].estado = ABERTO;
	}
}

/* emiteListagem - emite uma listagem dos aeroportos existentes, de acordo com o comando introduzido (0, 1 ou 2). */
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

/* indiceAeroporto - verifica se o aeroporto existe e retorna o seu indice, ou INEXISTENTE se nao existir */
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

	return INEXISTENTE;    /* se chegou aqui o aeroporto nao existe */	
}


/* printAeroportos - L 0: coloca no ecra o numero de voos de ida e de volta, mais a capacidade e id de todos os aeroportos na rede. */
void printAeroportos (aeroporto vet_aeroportos[], int numero_aeroportos)
{
	int i;

	for (i = 0; i < numero_aeroportos; i++)
	{
		printf("%s:%d:%d:%d\n", vet_aeroportos[i].id, vet_aeroportos[i].capacidade, vet_aeroportos[i].outgoing, vet_aeroportos[i].incoming);
	}
}


/* printAeroportoSorted - L 1: coloca no ecra o numero de voos de ida e de volta, mais a capacidade e id de todos os aeroportos na rede, por ordem alfabetica */
void printAeroportoSorted(aeroporto vet_aeroportos[], int numero_aeroportos)  /* utiliza o algoritmo selection sort */
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


/* printDistrVoosAeroportos - L 2: coloca no ecra quantos aeroportos tem n voos, por ordem crescente de n (numero de voos). */
void printDistrVoosAeroportos(aeroporto vet_aeroportos[], int numero_aeroportos)
{
	int vet_resultados[MAXVOOS], voos, i;
	
	for (i = 0; i < MAXVOOS; i++)
	{
		vet_resultados[i] = 0;
	}
	
	for (i = 0; i < numero_aeroportos; i++)
	{
		voos = vet_aeroportos[i].incoming + vet_aeroportos[i].outgoing;
		
		vet_resultados[voos] += 1;
	}
	
	for (i = 0; i < MAXVOOS; i++)
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
	
	if (i != INEXISTENTE && j != INEXISTENTE)
	{
		voos_aero1 = vet_aeroportos[i].incoming + vet_aeroportos[i].outgoing;
		voos_aero2 = vet_aeroportos[j].incoming + vet_aeroportos[j].outgoing;
		
		if (vet_aeroportos[i].estado == ABERTO && vet_aeroportos[j].estado == ABERTO)
		{
			num_voos = ida_volta ? 2 : 1;		/*Caso o voo seja de ida e volta, vai verificar se os aeroportos tem capacidade para mais 2 voos*/
												/*(num_voos = 2). Verifica se tem capacidade para mais um voo caso contrario (num_voos=1)*/
			if ((voos_aero1 + num_voos <= vet_aeroportos[i].capacidade) && (voos_aero2 + num_voos <= vet_aeroportos[j].capacidade))
			{
				matriz_voos[i][j] += 1;				
				vet_aeroportos[i].outgoing += 1;
				vet_aeroportos[j].incoming += 1;
				
				if (ida_volta)
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
	
	if (i != INEXISTENTE && j != INEXISTENTE)
	{
		if (vet_aeroportos[i].estado == ABERTO && vet_aeroportos[j].estado == ABERTO)
		{
			aero1_aero2 = matriz_voos[i][j];
			aero2_aero1 = matriz_voos[j][i];
		
			verifica_volta = ida_volta ? aero2_aero1 : 1; /*Se for para remover voo de ida e volta, vai verificar tambem se existem viagens do */
														/*aeroporto j para o aeroporto i. Verifica apenas do aeroporto i para j caso contrario.*/
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