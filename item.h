#ifndef ITEM_H
#define ITEM_H


//hashtag
typedef struct {
	char* name;
	int occur;
} hashtag;


hashtag* create_item(char* nome);
void update_item(hashtag* hash);
void write_item(hashtag* hash);
int compare_items(hashtag* h1, hashtag* h2);
int equal_items(hashtag* h1, hashtag* h2);
void free_item(hashtag* h);
int hash_item(hashtag* h, int M);

#endif