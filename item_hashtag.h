#ifndef ITEM_HASHTAG_H
#define ITEM_HASHTAG_H


#define MAXIMO 141

//hashtag
typedef struct {
	char name[MAXIMO];
	int occur;
} hashtag;


hashtag create_item(char nome[MAXIMO]);
void update_item(hashtag* hash);
void write_item(hashtag hash);
int compare_items(hashtag h1, hashtag h2);
int equal_items(hashtag h1, hashtag h2);

#endif