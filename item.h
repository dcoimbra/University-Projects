/*                  Grupo 18                   */
/* 57842 Filipa Marques - 84708 David Coimbra  */

/* Operacoes base para ADT hashtag */

#ifndef ITEM_H
#define ITEM_H

/* Estrutura hashtag */ 
typedef struct {
	char* name;
	int occur;
} hashtag;

hashtag* create_item(char* nome);
void update_item(hashtag* hash);
void write_item(hashtag* hash);
int compare_items(hashtag* h1, hashtag* h2);
int equal_items(hashtag* h1, hashtag* h2);
void greatest_item();
void free_item(hashtag* h);
int hash_item(hashtag* h, int M);

#endif