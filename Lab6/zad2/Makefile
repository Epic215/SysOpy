CC = gcc
CFLAGS = -Wall -std=c11

all :
	make view
	make calculator
	mkfifo PIPE1
	mkfifo PIPE2
view:
	$(CC) $(CFLAGS) view.c -o view
calculator:
	$(CC) $(CFLAGS) calculator.c -o calculator

clean :
	rm view
	rm calculator
	rm PIPE*

.PHONY : all interval_zad1 resetPomiary test clean