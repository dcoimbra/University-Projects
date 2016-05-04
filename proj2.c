/* Diretivas de pre-compilador */

// Bibliotecas standard
#include <stdio.h>
#include <stdlib.h>

// Definicoes de constantes
#define MAX 140
#define TRUE 1
#define FALSE 0

/* Prototipo auxiliar */
void inicializaMensagem(char* message);
void leMensagem(char* message);

/* Programa principal */

int main()
{
	char comando;

	while (TRUE)                 /* O programa pedira sempre um comando, */
	{                            /* exceto se algo correr mal ou o       */
		comando = getchar();     /* utilizador terminar o programa       */

		switch(comando)
		{
			/* Comando a - determina o numero de hashtags numa mensagem. */
			case 'a':
				/* funcao relativa ao comando a */
				break;

			/* Comando s - coloca no ecra o numero de hashtags distintas e o total de hashtags. */
			case 's':
				/* funcao relativa ao comando s */
				break;

			/* Comando l - lista todas as hashtags por ordem decrescente de ocorrencias, ou alfabeticamente em caso de igualdade. */
			case 'l':
				/* funcao relativa ao comando l */
				break;

			case 'x':
				return 0; /* programa terminado com sucesso */
		}

		getchar(); /* le o '\n' introduzido pelo utilizador */
	}

	return -1; /* programa terminado com erro - se chegou aqui algo correu mal */
}

void inicializaMensagem(char *message)
{
	message = (char*) malloc(sizeof(char)*(MAX+1));
}

void leMensagem(char *message) 
{
    int c, i = 0;
 
    while ((c = getchar()) != '\n' && c != EOF)
    {
        message[i++] = c;
    }
    
    message[i] = '\0';
}