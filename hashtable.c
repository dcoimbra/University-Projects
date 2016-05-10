#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "adt.h"
#include "item.h"

#define M 10007

typedef hashtag Item;

typedef struct node{
	Item* item;
	struct node*next;
} NODE;

typedef NODE *link;

link insertBeginList(link head_list, Item* item);
link searchList(link head_list, Item* item);
void writeList(int i, link head_list);
link greatestInList(link head_list);
void destroyList(link head_list);

link* createArrayFromHashtable();
void writeArray(link* array);
void quicksort(link* array, int l, int r);
int partition(link* array, int l, int r);

static int total_num_items = 0, num_diff_items = 0;
static link* heads;


void create()
{
	int i;
	heads = (link*) malloc(sizeof(link)*M);

	for (i = 0; i < M; i++)
		heads[i] = NULL;
} 

void insert(Item* item) 
{
	int i = hash_item(item, M);
	link l = searchList(heads[i], item);

	if (l)
	{
		update_item(l->item); 
		free_item(item);
	}

	else
	{
		heads[i] = insertBeginList(heads[i], item);
		num_diff_items++;
	}

	total_num_items++;
}

link insertBeginList(link head_list, Item* item)
{
	link t = (link) malloc(sizeof(NODE));
	t->item = item;
	t->next = head_list;

	return t;	
}

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
/*
void write()
{
	int i;
	
	for (i = 0; i < M; i++)
	{
		writeList(i, heads[i]);
	}
}
void writeList(int i, link head_list)
{
	if (head_list == NULL)
	{
		return;
	}

	int j;
	
	link t = head_list;
	
	//printf("lista %d:\n", i);
	
	for (j = 1; t != NULL; t = t->next, j++)
	{
		//printf("%do: ", j);
		write_item(t->item);
	}

	//printf("\n");
}
*/

/* faz print do numero total de items diferentes e do numero total de items.*/
void count()
{
	printf("%d %d\n", num_diff_items, total_num_items);
}


void greatest()
{
	link max = NULL, max_in_list;
	int i;
	
	for(i = 0; i < M; i++)
	{
		max_in_list = greatestInList(heads[i]);
		
		if (max_in_list != NULL)
		{	
			if ((max == NULL) || (compare_items(max_in_list->item, max->item) < 0))
			{
				max = max_in_list;
			}
		}
	}
	
	if (max != NULL)
	{
		write_item(max->item);
		return;
	}
	
	//printf("greatest: hashtable vazia.\n");
}

/* faz print do maior item na lista. Caso a lista esteja vazia nao faz nada.*/
link greatestInList(link head_list)
{
	if (head_list == NULL)
		return NULL;

	link t, max = head_list;
	
	for (t = head_list->next; t != NULL;t = t->next)
	{
		if (compare_items(t->item, max->item) < 0)
		{
			max = t;
		}
	}
	
	return max;
}

void destroy()
{
	int i;
	for(i = 0; i < M; i++)
	{
		destroyList(heads[i]);
	}
	
	free(heads);
}

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

void writeSorted()
{
	link* array = createArrayFromHashtable();
	quicksort(array, 0, num_diff_items-1);
	writeArray(array);
	free(array);
}

link* createArrayFromHashtable()
{
	int i, j = 0;
	link* array = (link*)malloc(sizeof(link*)*num_diff_items);

	for(i=0; i < num_diff_items; i++)
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

void writeArray(link* array)
{
	int j = 0;

	for (j = 0; j < num_diff_items; j++)
	{
		//printf("%d:\n", j);
		write_item(array[j]->item);
				
	}
	//printf("\n");
}

void quicksort(link* a, int l, int r)
{
	int i;
	if (r <= l)
		return;

	i = partition(a, l, r);
	quicksort(a, l, (i-1));
	quicksort(a, (i+1), r);
}

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


