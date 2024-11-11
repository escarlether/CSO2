CC = gcc
CFLAGS = -Wall -g -fsanitize=address
LDFLAGS = 


all: split

.PHONY: all clean

clean:
	rm --force split split.o main.o

split: split.o main.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o split split.o main.o

split.o: split.h split.c
	$(CC) $(CFLAGS) -c split.c -o split.o

main.o: main.c split.h
	$(CC) $(CFLAGS) -c main.c -o main.o 
