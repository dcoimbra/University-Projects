/*                  Grupo 18                   */
/* 57842 Filipa Marques - 84708 David Coimbra  */

/* Diretivas de pre-compilador */

//Bibliotecas standard
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Outras bibliotecas
#include "item.h"

/* Ponteiro global greatest: aponta para o hashtag mais popular. */
static hashtag* greatest = NULL;

/* Prototipo */
hashtag* create_item(char* nome);
void update_item(hashtag* hash);
void write_item(hashtag* hash);
int compare_items(hashtag* h1, hashtag* h2);
int equal_items(hashtag* h1, hashtag* h2);
void greatest_item();
int hash(char *v, int M);
int hash_item(hashtag* h, int M);
void free_item(hashtag* h);

/* create_item: cria um novo item com o nome dado */
hashtag* create_item(char* nome)
{
	hashtag* new = (hashtag*) malloc(sizeof(hashtag));
	new->name = (char*) malloc(sizeof(char)*(strlen(nome)+1));

	strcpy(new->name, nome);
	new->occur = 1;

	if (greatest == NULL || compare_items(new, greatest) < 0)
	{
		greatest = new;
	}

	return new;
}

/* update_item: atualiza o numero de ocorrencias do item dado. */
void update_item(hashtag* hash)
{
	hash->occur++;

	if (compare_items(hash, greatest) < 0)
	{
		greatest = hash;
	}
}

/* write_item: devolve o nome do hashtag dado. */
void write_item(hashtag* hash)
{
	printf("%s %d\n", hash->name, hash->occur);
}

/* compare_items: retorna o resultado de comparar o numero
				  de ocorrencias de 2 items: maior que 0
				  se h2 > h1 e menor caso contrario. Devolve 0
				  se forem iguais. */
int compare_items(hashtag* h1, hashtag* h2)
{
	int comp_occur = (h2->occur)-(h1->occur);

	return comp_occur == 0 ? strcmp(h1->name, h2->name) : comp_occur;
}

/* equal_items: retorna 0 se os dois items tiverem nomes diferentes e um valor
                diferente de 0 caso contrario. */
int equal_items(hashtag* h1, hashtag* h2)
{
	return strcmp(h1->name, h2->name) == 0;
}

/* greatest_item: coloca no ecra o item mais popular */
void greatest_item()
{
	if (greatest != NULL)
		write_item(greatest);
}

/* hash: associa um hash a um item e devolve esse hash */
int hash(char *v, int M)
{
	int h, a = 31415, b = 27183;

	for (h = 0; *v != '\0'; v++, a = a*b % (M-1))
		h = (a*h + *v) % M;

	return h;
}

/* hash_item: retorna o hash associado ao nome do item dado. */
int hash_item(hashtag* h, int M)
{
	return hash(h->name, M);
}

/* free_item: liberta a memoria alocada para um item. */
void free_item(hashtag* h)
{
	free(h->name);
	free(h);
	h = NULL;
}