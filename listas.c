#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "listas.h"
#include "item_hashtag.h"

typedef hashtag Item;

//estruturas para as listas
typedef struct node{
	Item item;
	struct node*next;
} node;

typedef node* link;

void create();
void insert();
void write();
void count();
link find();
link greatest();

//criar uma head e uma tail para a lista
static link head, tail;
static int num_diff_items = 0, total_num_items = 0;

int main()
{
	create();
	hashtag hash_ola = create_item("#ola");
	insert(hash_ola); 
	insert(create_item("#teste"));
	hashtag hash_adeus = create_item("#adeus");
	insert(hash_adeus);
	write();
	printf("\n");
	hashtag hash_mentiroso = create_item("#ola");
	insert(hash_mentiroso);
	write();
	link great = greatest();
	write_item(great->item);
	count();

	return 0;
}

/* cria uma nova lista */
void create()
{
	head = NULL;
}

/* insere um novo elemento no fim da lista*/
void insert(Item item)
{
	link t = (link) malloc(sizeof(node));
	t->item = item;
	t->next = NULL;

	if (head == NULL)
	{
		head = tail = t;
		num_diff_items = total_num_items = 1;
		free(t);
		return;
	}
	
	link l = find(item);
	
	if (l)
	{
		update_item(&(l->item)); /*endereco de memoria do item dentro da lista*/
		
		total_num_items++;
		free(t);
		return;
	}
	
	tail->next = t;
	tail = tail->next;
	num_diff_items++;
	total_num_items++;
	free(t);
}

/* faz print da lista criada.*/
void write()
{
	if (head == NULL)
	{
		printf("nao existe nenhuma lista.");
		
		return;
	}

	int i;
	
	link t = head;
	
	for (i = 1; t != NULL; t = t->next, i++)
	{
		printf("elemento %d:\n", i);
		write_item(t->item);
	}
	
	printf("\n");
}

void count()
{
	printf("%d %d\n", num_diff_items, total_num_items);
}

/*devolve 1 se o elemento n existir na lista, 0 c.c.*/
link find(Item item)
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

link greatest()
{
	if (head == NULL)
	
	return NULL;

	link t, max = head;
	
	for (t = head->next; t != NULL;t = t->next)
	{
		if (compare_items(t->item, max->item) < 0)
			max = t;
	}
	
	return max;
}