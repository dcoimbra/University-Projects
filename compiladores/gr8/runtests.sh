#!/bin/bash

for source in tests-gr8-daily-201803071132/*.gr8; do 

	./gr8 --target asm "$source"

done

for asm in tests-gr8-daily-201803071132/*.asm; do

	yasm -felf32 "$asm"
done

n=A

for obj in *ok.o; do

	ld -m elf_i386 -o "file$n" "$obj" -L$HOME/compiladores/root/usr/lib -lrts

	let "n+=1"
done

i=A

for exec in file*; do

	echo "$exec"

	cat "out$i"

	./"$exec" > "out$i"

	echo ""

	let "i+=1"

	echo "------------------------TESTE REALIZADO-------------------------------"
done

rm file*
rm out*

# © All Rights Reserved to Bernardo Andrade Compiladores 2017-2018 
#   Adaptado por David Coimbra a 20 de Maio de 2018