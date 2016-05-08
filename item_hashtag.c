#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "item.h"

//cria um novo item do tipo hashtag
hashtag create_item(char nome[])
{
	hashtag new;
	strcpy(new.name, nome);
	new.occur = 1;

	return new;
}

//atualiza o numero de ocorrencias de um #
void update_item(hashtag* hash)
{
	hash->occur++;
}

//devolve o #nome.
void write_item(hashtag hash)
{
	printf("%s %d\n", hash.name, hash.occur);
}

int compare_items(hashtag h1, hashtag h2)
{
	int comp_occur = h2.occur-h1.occur;
	int comp_name = strcmp(h1.name, h2.name);
	
	return comp_occur == 0 ? comp_name : comp_occur;
}

int equal_items(hashtag h1, hashtag h2)
{
	return strcmp(h1.name, h2.name) == 0;
}