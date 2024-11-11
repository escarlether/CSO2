CC = clang
CFLAGS = -Wall -O1
LDFLAGS = 


all: gettimings

.PHONY: all clean

clean:
	rm --force gettimings gettimings.o

gettimings: gettimings.o
	$(CC) $(LDFLAGS) -o gettimings gettimings.o

gettimings.o: gettimings.c
	$(CC) $(CFLAGS) -c gettimings.c -o gettimings.o