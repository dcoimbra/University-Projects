/* Diretivas de pre-compilador */

// Bibliotecas standard
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Definicoes de constantes
#define MAX 140
#define NUMSEP 11
#define TRUE 1
#define FALSE 0

/* Estruturas */

//hashtag
typedef struct {
	int ocur;
	char name[MAX];
} hashtag;

/* Prototipo principal */

void processMessage();

/* Prototipo auxiliar */

void readMessage(char *message);
void split(char *line);

/* Programa principal */

static int Ntotal = 0;

int main()
{
	char comm;

	while (TRUE)                 /* O programa pedira sempre um comando, */
	{                            /* exceto se algo correr mal ou o       */
		comm = getchar();        /* utilizador terminar o programa       */

		getchar(); /* le o ' ' introduzido pelo utilizador */
		
		switch(comm)
		{
			/* Comando a - determina o numero de hashtags numa mensagem. */
			case 'a':
				processMessage();
				break;

			/* Comando s - coloca no ecra o numero de hashtags distintas e o total de hashtags. */
			case 's':
				/* funcao relativa ao comando s */
				printf("%d\n", Ntotal);
				break;

			/* Comando l - lista todas as hashtags por ordem decrescente de ocorrencias, ou alfabeticamente em caso de igualdade. */
			case 'l':
				/* funcao relativa ao comando l */
				break;

			case 'x':
				return 0; /* programa terminado com sucesso */

			default:
				printf("Comando invalido\n");
		}
	}

	return -1; /* programa terminado com erro - se chegou aqui algo correu mal */
}

void processMessage()
{
	char* message = (char*) malloc(sizeof(char)*(MAX+1));
	
	readMessage(message);
	split(message);
	free(message);
}

void readMessage(char *message) 
{
    int c, i = 0;
 
    while ((c = getchar()) != '\n' && c != EOF)
    {
        message[i++] = c;
    }

    message[i] = '\0';
}

void split(char *line)
{
    char separators[] = {' ','\t',',',';','.','?','!','"','\n',':','\0'};
    
    int i, l = strlen(line);

    for (i = 0; i != l; i++)
    {
    	line[i] = tolower(line[i]);
    }

    char *token = strtok(line, separators);
    
    while (token!=NULL)
    { 
    	if (token[0] == '#')
    	{
    		Ntotal++;
    	}

    	token = strtok(NULL, separators); 
    } 
} 