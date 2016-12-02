CFLAGS = -g -Wall -pedantic

all: i-banco i-banco-terminal

i-banco: i-banco.o contas.o
	gcc -o i-banco $(CFLAGS) -pthread contas.o i-banco.o

i-banco.o: i-banco.c 
	gcc -c -pthread $(CFLAGS) i-banco.c

i-banco-terminal: i-banco-terminal.o commandlinereader.o
	gcc -o i-banco-terminal $(CFLAGS) i-banco-terminal.o commandlinereader.o

i-banco-terminal.o: i-banco-terminal.c
	gcc -c $(CFLAGS) i-banco-terminal.c

contas.o: contas.c contas.h
	gcc -c $(CFLAGS) contas.c contas.h

commandlinereader.o: commandlinereader.c commandlinereader.h
	gcc -c $(CFLAGS) commandlinereader.c commandlinereader.h
	