CC = gcc
CFLAGS = -Wall -std=c11

all :
	make makeProcesses
makeProcesses:
	$(CC) $(CFLAGS) makeProcesses.c -o makeProcesses
test :
	make all
	./makeProcesses 3
clear :
	rm makeProcesses

.PHONY : all makeProcesses test clear