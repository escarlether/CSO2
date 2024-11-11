CC = clang
CFLAGS = -Wall -O1
LDFLAGS = -Wall

.PHONY: all clean

all: mlpt

mlpt: mlpt.o libmlpt.a
	$(CC) $(LDFLAGS) -o mlpt mlpt.o libmlpt.a -L. -lmlpt

mlpt.o: mlpt.c mlpt.h config.h
	$(CC) $(CFLAGS) -o mlpt.o -c mlpt.c

libmlpt.a: mlpt.o 
	ar -rcs libmlpt.a mlpt.o 

clean:
	rm --force libmlpt.a mlpt.o
