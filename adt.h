
#include "item.h"


typedef hashtag Item;

//estruturas para as listas
typedef struct node{
	Item item;
	struct node*next;
} NODE;

typedef NODE* link;


void create();
void insert();
void write();
void count();
link find();
link greatest();
