/*                  Grupo 18                   */
/* 57842 Filipa Marques - 84708 David Coimbra  */

/* Operacoes base para tabela de dispersao */

/* Diretivas de pre-compilador */

// Bibliotecas standard
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Outras bibliotecas
#include "adt.h"
#include "item.h"

// Definicoes de constantes
#define M 13337 //numero de listas ligadas numa tabela de dispersao

//Estruturas
typedef hashtag Item;

//NODE
typedef struct node {
	Item* item;
	struct node* next;
} NODE;

typedef NODE *link;

/* Variaveis globais */
static int total_num_items = 0, num_diff_items = 0;
static link* heads; /* tabela de dispersao */

/* Prototipo */
void create();
void insert(Item* item);
link insertBeginList(link head_list, Item* item);
link searchList(link head_list, Item* item);
void count();
void destroy();
void destroyList(link head_list);
void writeSorted();
link* createArrayFromHashtable();
void writeArray(link* array);
void quicksort(link* array, int l, int r);
int partition(link* array, int l, int r);

/* create: inicializa uma tabela de dispersao. */
void create()
{
	int i;
	heads = (link*) malloc(sizeof(link)*M);

	for (i = 0; i < M; i++)
		heads[i] = NULL;
} 

/* insert: insere na tabela de dispersao o ponteiro para item recebido. */
void insert(Item* item) 
{
	int i = hash_item(item, M);
	link l = searchList(heads[i], item);

	/* se o item existir na lista, incrementa o respetivo numero de ocorrencias. */
	if (l)
	{
		update_item(l->item); 
		free_item(item);
	}

	/* caso contrario, insere-o no inicio de uma lista. */
	else
	{
		
		heads[i] = insertBeginList(heads[i], item);
		num_diff_items++;
	}

	total_num_items++;
}

/* insertBeginList: insere um item no inicio de uma lista da tabela de dispersão. 
	devolve o novo elemento correspondente 'a nova cabeca da lista. */
link insertBeginList(link head_list, Item* item)
{
	link t = (link) malloc(sizeof(NODE));
	t->item = item;
	t->next = head_list;

	return t;	
}

/* searchList: procura um item numa lista e devolve o elemento que contem esse item. */
link searchList(link head_list, Item* item)
{
	if (head_list == NULL)
		return NULL;

	link t;

	for (t = head_list; t != NULL; t = t->next)
	{
		if (equal_items(t->item, item))
			return t;
	}

	return NULL;
}

/* count - coloca no ecra o numero total de items diferentes e do numero total de items.*/
void count()
{
	printf("%d %d\n", num_diff_items, total_num_items);
}

/* destroy - liberta toda a memoria alocada para a tabela de dispersao */
void destroy()
{
	int i;
	for(i = 0; i < M; i++)
	{
		destroyList(heads[i]);
	}
	
	free(heads);
}

/* destroyList - liberta a memoria alocada para uma lista */
void destroyList(link head_list)
{
	if (head_list == NULL)
	{
		return;
	}

	link t, tfree;
	
	for (t = head_list; t != NULL;)
	{
		tfree = t;
		t = t->next;

		free_item(tfree->item);
		free(tfree);
	}
}

/* writeSorted() - escreve no ecra os elementos de um vetor ordenado,
				   criado a partir de uma tabela de dispersao. */
void writeSorted()
{
	link* array = createArrayFromHashtable();
	quicksort(array, 0, num_diff_items-1);
	writeArray(array);
	free(array);
}

/* createArrayFromHashtable - devolve um array de ponteiros, que apontam para cada um dos elementos da hashtable. */
link* createArrayFromHashtable()
{
	int i, j = 0;
	link* array = (link*)malloc(sizeof(link*)*num_diff_items);

	for(i = 0; i < num_diff_items; i++)
		array[i] = NULL;

	for (i = 0; i < M; i++)
	{
		if (heads[i] != NULL)
		{
			link t = heads[i];
			for (; t != NULL; t = t->next, j++)
			{
				array[j] = t;
			}
		}
	}
	return array;
}

/* writeArray - escreve no ecra os items para os quais os ponteiros de um vetor estao a apontar. */
void writeArray(link* array)
{
	int j = 0;

	for (j = 0; j < num_diff_items; j++)
	{
		write_item(array[j]->item);
	}
}

/* quicksort - ordena um vetor segundo o algoritmo quicksort. */
void quicksort(link* a, int l, int r)
{
	int i;
	if (r <= l)
		return;

	i = partition(a, l, r);
	quicksort(a, l, (i-1));
	quicksort(a, (i+1), r);
}

/* partition - como parte do algoritmo quicksort, divide um vetor em duas partes
			   segundo um pivot. */ 
int partition(link* array, int l, int r) 
{
	int i = l-1;
	int j = r;
	link aux;
	link v = array[r];
	
	while (i < j) 
	{
		while (compare_items(array[++i]->item, v->item) < 0);
		while (compare_items(v->item, array[--j]->item) < 0)
			if (j == l)
				break;
			
			if (i < j)
			{
				aux = array[i];
				array[i] = array[j];
				array[j] = aux;
			}
	}
	
	aux = array[i];
	array[i] = array[r];
	array[r] = aux;

	return i;
}