CC = gcc
CFLAGS = -Wall

all: printers users

printers:
	$(CC) $(CFLAGS) users.c -o users
users:
	$(CC) $(CFLAGS) printers.c -o printers

clean :
	rm users
	rm printers

.PHONY : all printers users clean