CC = gcc
CFLAGS = -Wall -std=c11

all :
	make main
main:
	$(CC) $(CFLAGS) main.c -o main
test :
	make all
	./main /home
clear :
	rm main

.PHONY : all main test clear