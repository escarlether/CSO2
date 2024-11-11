CC = clang
CFLAGS = -Wall
LDFLAGS = -


all:my_system.o

.PHONY: all clean

clean:
	rm --force my_system my_system.o main.o

my_system.o: my_system.c
	$(CC) $(CFLAGS) -c my_system.c -o my_system.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o
