/*                  Grupo 25                  */
/* 57842 Filipa Marques - 84708 David Coimbra */
                                            
/* Diretivas de pre-compilador */

// Bibliotecas standard
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Outras Bibliotecas
#include "adt.h"
#include "item.h"

// Definicoes de constantes
#define MAX 140   /* Tamanho maximo de uma mensagem */
#define NUMSEP 11 
#define TRUE 1
#define FALSE 0

/* Prototipo principal */

void processMessage();

/* Prototipo auxiliar */

void readMessage(char *message);
void split(char *line);

/* Programa principal */

int main()
{
	char comm;
	create();

	while (TRUE)                 /* O programa pedira sempre um comando, */
	{   						 /* exceto se algo correr mal ou o       */
		comm = getchar();        /* utilizador terminar o programa       */                                

		getchar(); /* le o espaco introduzido pelo utilizador */

		switch(comm)
		{
			/* Comando a - processa uma mensagem, extraindo as hashtags nela. */
			case 'a':
				processMessage();
				break;

			/* Comando s - coloca no ecra o numero de hashtags distintas e o total de hashtags. */
			case 's':
				count();
				break;

			/* Comando m - coloca no ecra a hashtag mais popular e o seu numero de ocorrencias. */
			case 'm':
				greatest_item();
				break;

			/* Comando l - lista todas as hashtags por ordem decrescente de ocorrencias, ou alfabeticamente em caso de igualdade. */
			case 'l':
				/* funcao relativa ao comando l */
				writeSorted();
				break;

			/* Comando x - termina o programa. */
			case 'x':
				destroy();
				return 0; /* programa terminado com sucesso e libertada toda a memoria alocada. */

			default:
				printf("Comando invalido\n");
		}
	}

	return -1; /* programa terminado com erro - se chegou aqui algo correu mal */
}

/* processMessage - le uma mensagem e processa-a. */
void processMessage()
{
	char* message = (char*) malloc(sizeof(char)*(MAX+1));
	
	readMessage(message);
	split(message);
	free(message);
}

/* readMessage -  le texto do terminal, colocando-o
				  na memoria alocada indicada pelo */
void readMessage(char *message) 
{
    int c, i = 0;
 
    while ((c = getchar()) != '\n' && c != EOF)
    {
        message[i++] = c;
    }

    message[i] = '\0';
}

/* split - recebe uma mensagem e divide-a em tokens. 
		   Os tokens que forem hashtags são inseridos na tabela de dispersao. */
void split(char *line)
{
    char separators[] = {' ','\t',',',';','.','?','!','"','\n',':','\0'};
    
    int i, l = strlen(line);

    for (i = 0; i < l; i++)
    {
    	line[i] = tolower(line[i]);
    }

    char *token = strtok(line, separators);
    
    while (token != NULL)
    { 
    	if (token[0] == '#' && strlen(token) > 1)
    	{
    		hashtag* new_hashtag = create_item(token);
			
			insert(new_hashtag);
    	}

    	token = strtok(NULL, separators); 
    } 
}