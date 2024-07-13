CC = gcc
CFLAGS = -Wall -std=c11

all :
	make catcher
	make sender
catcher:
	$(CC) $(CFLAGS) catcher.c -o catcher
sender:
	$(CC) $(CFLAGS) sender.c -o sender
clear :
	rm catcher
	rm sender

.PHONY : all sender catcher clear
