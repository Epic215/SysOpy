CC = gcc
CFLAGS = -Wall -std=c11

all :
	make reverse_byte
	make reverse_block
reverse_byte :
	$(CC) $(CFLAGS) reverseByByte.c -o reverseByte
reverse_block:
	$(CC) $(CFLAGS) reverseByBlock.c -o reverseBlock
resetPomiary :
	touch pomiar_zad_2.txt
	rm  pomiar_zad_2.txt
	touch pomiar_zad_2.txt
test :
	$(CC) $(CFLAGS) random_txt_files.c -o randomfilestxt
	./randomfilestxt
	make all
	make resetPomiary
	./reverseByte random10.txt random10_result.txt
	./reverseBlock random10.txt random10_result.txt
	./reverseByte random1025.txt random10_result.txt
	./reverseBlock random10.txt random10_result.txt
	./reverseByte random2048.txt random10_result.txt
	./reverseBlock random2048.txt random10_result.txt
clear :
	rm *txt
	rm reverseByte
	rm reverseBlock

.PHONY : reverse_byte reverse_block all clear test resetPomiary
