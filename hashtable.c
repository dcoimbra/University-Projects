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
link append_lists(link head1, link head2);

static int total_num_items = 0, num_diff_items = 0;
static link *heads;

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

/*link find(Item* item)
{
	int i = hash_item(item, M);
	return searchList(heads[i], item);
}*/

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
	
	printf("lista %d:\n", i);
	
	for (j = 1; t != NULL; t = t->next, j++)
	{
		printf("el %d: ", j);
		write_item(t->item);
	}

	printf("\n");
}

/* faz print do numero total de items diferentes e do numero total de items.*/
void count()
{
	printf("%d %d\n", num_diff_items, total_num_items);
}

int totalDiffItems()
{
	return num_diff_items;
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
	
	printf("greatest: hashtable vazia.\n");
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

link append_lists(link head1, link head2)
{	
	if (head1 == NULL)
		return head2;
    
    if (head2 == NULL)
    	return head1;
               
    link t;
    
    t = head1;           
    while (t->next != NULL)
    {
    	printf("duvido, mas cheguei aqui\n");
    	t = t->next;
    }
    
    t->next = head2;
    
    return head1;
}

void append_all_lists()
{
	int i;
	link all = NULL;
	
	for (i = 0; i < M; i++)
	{
		if (heads[i] != NULL)
		{
			printf("print aqui\n");
			write();
			all = append_lists(all, heads[i]);
			printf("-------%d-------\n", i);
			write();
		}
	}

	for (i = 0; i < num_diff_items; i++)
	{
		printf("%d\n", num_diff_items);
		write_item(all[i].item);
	}

	printf("\n");
}