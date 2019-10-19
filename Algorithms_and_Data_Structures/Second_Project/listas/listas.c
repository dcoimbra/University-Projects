#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "adt.h"
#include "item.h"

/*------------------------------*/
//estruturas para as listas

typedef hashtag Item;


typedef struct node{
	Item* item;
	struct node*next;
} NODE;

typedef NODE* link;

/*------------------------------*/
/*----------PROTOTIPO-----------*/
/*------(funcoes internas)------*/
/*------------------------------*/
link find();


/*------------------------------*/
/*-------VARIAVEIS GLOBAIS------*/
/*------------------------------*/
//criar uma head e uma tail para a lista
static link head, tail;

static int num_diff_items = 0, total_num_items = 0;

/*------------------------------*/
/*------FUNCOES DE LISTAS-------*/
/*------------------------------*/

/* cria uma nova lista */
void create()
{
	head = tail = NULL;
}


/* insere um novo elemento no fim da lista*/
void insert(Item* item)
{
	link l = find(item);
	
	if (l)
	{
		update_item(l->item); /*endereco de memoria do item dentro da lista*/
		free_item(item);
		total_num_items++;
		return;
	}


	link t = (link) malloc(sizeof(NODE));
	t->item = item;
	t->next = NULL;

	if (head == NULL)
	{
		head = tail = t;
		num_diff_items = total_num_items = 1;
		
		return;
	}
	
	tail->next = t;
	tail = tail->next;
	num_diff_items++;
	total_num_items++;
	
}


/* faz print da lista criada.*/
void write()
{
	if (head == NULL)
	{
		printf("nao existe nenhuma lista.\n");
		
		return;
	}

	int i;
	
	link t = head;
	
	for (i = 1; t != NULL; t = t->next, i++)
	{
		printf("elemento %d:\n", i);
		write_item(t->item);
	}
}


/* faz print do numero total de items diferentes e do numero total de items.*/
void count()
{
	printf("%d %d\n", num_diff_items, total_num_items);
}


/*devolve 1 se o elemento n existir na lista, 0 c.c.*/
link find(Item* item)
{
	if (head == NULL)
		return NULL;

	link t;

	for (t = head; t != NULL; t = t->next)
	{
		if (equal_items(t->item, item))
			return t;
	}

	return NULL;
}


/* faz print do maior item na lista. Caso a lista esteja vazia nao faz nada.*/
void greatest()
{
	if (head == NULL)
		return;

	link t, max = head;
	
	for (t = head->next; t != NULL;t = t->next)
	{
		if (compare_items(t->item, max->item) < 0)
			max = t;
	}

	write_item(max->item);
}

void destroy()
{
	if (head == NULL)
	{
		return;
	}

	link t, tfree;
	
	for (t = head; t != NULL;)
	{
		tfree = t;
		t = t->next;

		free_item(tfree->item);
		free(tfree);
	}

}