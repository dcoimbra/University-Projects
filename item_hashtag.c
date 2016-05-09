#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "item.h"

//cria um novo item do tipo hashtag
hashtag* create_item(char* nome)
{
	hashtag* new = (hashtag*) malloc(sizeof(hashtag));
	new->name = (char*) malloc(sizeof(char)*(strlen(nome)+1));

	strcpy(new->name, nome);
	new->occur = 1;

	return new;
}

//atualiza o numero de ocorrencias de um #
void update_item(hashtag* hash)
{
	hash->occur++;
}

//devolve o #nome.
void write_item(hashtag* hash)
{
	printf("%s %d\n", hash->name, hash->occur);
}

int compare_items(hashtag* h1, hashtag* h2)
{
	int comp_occur = (h2->occur)-(h1->occur);
		
	return comp_occur == 0 ? strcmp(h1->name, h2->name) : comp_occur;
}

int equal_items(hashtag* h1, hashtag* h2)
{
	return strcmp(h1->name, h2->name) == 0;
}

int hash(char *v, int M)
{
	int h, a = 31415, b = 27183;

	for (h = 0; *v != '\0'; v++, a = a*b % (M-1))
		h = (a*h + *v) % M;

	return h;
}

int hash_item(hashtag* h, int M)
{
	return hash(h->name, M);
}

void free_item(hashtag* h)
{
	free(h->name);
	free(h);
	h = NULL;
}
