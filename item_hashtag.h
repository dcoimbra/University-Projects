#define MAX 141

//hashtag
typedef struct {
	char name[MAX];
	int occur;
} hashtag;


hashtag create_item(char nome[MAX]);
void update_item(hashtag* hash);
void write_item(hashtag hash);
int compare_items(hashtag h1, hashtag h2);
int equal_items(hashtag h1, hashtag h2);
