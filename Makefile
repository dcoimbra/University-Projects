CFLAGS = -g -Wall -pedantic

i-banco: i-banco.o contas.o commandlinereader.o
	gcc -o i-banco $(CFLAGS) -pthread contas.o commandlinereader.o i-banco.o

i-banco.o: i-banco.c
	gcc -c -pthread $(CFLAGS) i-banco.c

contas.o: contas.c contas.h
	gcc -c $(CFLAGS) contas.c contas.h

commandlinereader.o: commandlinereader.c commandlinereader.h
	gcc -c $(CFLAGS) commandlinereader.c commandlinereader.h