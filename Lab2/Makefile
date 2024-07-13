CC = gcc
CFLAGS = -Wall -std=c11 -O0
all :
	make static_client
	make shared_client
	make dynamic_client
staticlib :
	$(CC) $(CFLAGS) -c collatz.c
	ar rcs libstaticcollatz.a collatz.o
sharedlib :
	$(CC) -o libsharedcollatz.so -fpic -shared collatz.c
static_client :
	make staticlib
	$(CC) staticclient.c -o staticClient -L. -lstaticcollatz
shared_client :
	make sharedlib
	$(CC) -c sharedclient.c -o sharedclient.o
	$(CC) sharedclient.o -o sharedClient -lsharedcollatz -L. -Wl,-rpath,.
dynamic_client :
	make sharedlib
	nm -D libsharedcollatz.so
	$(CC) dynamicclient.c -o dynamicClient -ldl
clear :
	rm *.o
	rm *.so
	rm *.a
	rm *Client
test :
	make all
	./staticClient
	./sharedClient
	./dynamicClient

.PHONY : staticlib, sharedlib, static_client, shared_client, dynamic_client, all, clear, test