CC = gcc
CFLAGS = -Wall -std=c11

all :
	make serwer
	make client
serwer:
	$(CC) $(CFLAGS) server.c -o serwer
client :
	$(CC) $(CFLAGS) client.c -o client


clean :
	rm serwer
	rm client

.PHONY : all serwer client clean
