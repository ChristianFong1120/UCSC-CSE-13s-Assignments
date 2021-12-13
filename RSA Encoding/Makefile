CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra $(shell pkg-config --cflags gmp)
LFLAGS = $(shell pkg-config --libs gmp)

all: decrypt keygen encrypt
decrypt: decrypt.o numtheory.o randstate.o rsa.o
	$(CC) decrypt.o numtheory.o randstate.o rsa.o $(LFLAGS) -lm -g -o decrypt

decrypt.o: decrypt.c numtheory.c randstate.c rsa.c
	$(CC) $(CFLAGS) -c -g decrypt.c numtheory.c randstate.c rsa.c
encrypt: encrypt.o numtheory.o randstate.o rsa.o
	$(CC) encrypt.o numtheory.o randstate.o rsa.o $(LFLAGS) -lm -g -o encrypt


encrypt.o: encrypt.c numtheory.c randstate.c rsa.c
	$(CC) $(CFLAGS) -c -g encrypt.c numtheory.c randstate.c rsa.c

keygen: keygen.o numtheory.o randstate.o rsa.o
	$(CC) keygen.o numtheory.o randstate.o rsa.o $(LFLAGS) -lm -g -o keygen

keygen.o: keygen.c numtheory.c randstate.c rsa.c
	$(CC) $(CFLAGS) -c -g keygen.c numtheory.c randstate.c rsa.c

numtheory: numtheory.o randstate.h numtheory.h
	$(CC)  numtheory.o $(LFLAGS) -o numtheory

numtheory.o: numtheory.c  numtheory.h
	$(CC) $(CFLAGS) -c numtheory.c 

randstate:  randstate.o randstate.h
	$(CC)  randstate.o $(LFLAGS) -o randstate

randstate.o: randstate.c randstate.h
	$(CC) $(CFLAGS) -c randstate.c

rsa: rsa.o numtheory.h randstate.h
	$(CC) -o rsa rsa.o $(LFLAGS)

rsa.o: rsa.c rsa.h
	$(CC) $(CFLAGS) -c  rsa.c 

clean:
	rm -f decrypt encrypt keygen decrypt.o encrypt.o keygen.o numtheory.o randstate.o rsa.o

format:
	clang-format -i -style=file *.[ch]

