CC = gcc
CFLAGS = -Wall -std=c11

all :
	make interval_zad1
	touch pomiar_zad_1.txt
interval_zad1:
	$(CC) $(CFLAGS) interval_zad1.c -o interval_zad1 -lm
resetPomiary :
	touch pomiar_zad_1.txt
	rm  pomiar_zad_1.txt
	touch pomiar_zad_1.txt
test :
	make all
	make resetPomiary
	./interval_zad1 0.0001 10000
	./interval_zad1 0.01 1
	./interval_zad1 0.0000001 10000
	./interval_zad1 0.0000001 333
	./interval_zad1 0.01 3
	./interval_zad1 0.001 10
	./interval_zad1 0.01 100
clean :
	rm *txt
	rm interval_zad1

.PHONY : all interval_zad1 resetPomiary test clean
