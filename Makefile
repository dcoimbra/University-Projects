CFLAGS = -g -Wall -pedantic

i-banco: contas.c contas.h i-banco.c commandlinereader.c commandlinereader.h
	gcc -o i-banco $(CFLAGS) contas.c commandlinereader.c i-banco.c