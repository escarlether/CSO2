CC = clang
CFLAGS = -Wall
LDFLAGS = -fsanitize=address


all: guesser

.PHONY: all clean

clean:
	rm --force cheer.o grunt.o guesser.o libsay.a guesser

guesser: libsay.a guesser.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o guesser guesser.o libsay.a

libsay.a: cheer.o grunt.o
	ar rv libsay.a cheer.o grunt.o

guesser.o: say.h guesser.c
	$(CC) $(CFLAGS) -c guesser.c -o guesser.o

grunt.o: grunt.c say.h
	$(CC) $(CFLAGS) -c -o grunt.o grunt.c

cheer.o: cheer.c say.h
	$(CC) $(CFLAGS) -c -o cheer.o cheer.c
