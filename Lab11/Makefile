CC = gcc
CFLAGS = -Wall

all: serwer client

client:
	$(CC) $(CFLAGS) client.c -o client

serwer:
	$(CC) $(CFLAGS) serwer.c -o serwer

clean :
	rm client
	rm serwer

.PHONY : all serwer client clean
