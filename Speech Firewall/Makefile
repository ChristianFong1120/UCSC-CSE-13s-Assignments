CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra

all: banhammer

parser: parser.o parser.h
	$(CC) parser.o -o parser

parser.o: parser.c parser.h
	$(CC) $(CFLAGS) -c parser.c


speck: speck.o salts.h speck.h
	$(CC) speck.o -o speck

speck.o: speck.c speck.h
	$(CC) $(CFLAGS) -c speck.c

ht: ht.o ht.h salts.h speck.o bst.o node.o
	$(CC) ht.o speck.o bst.o node.o -o ht

ht.o: ht.c ht.h speck.c bst.c node.c
	$(CC) $(CFLAGS) -c ht.c speck.c bst.c node.c

bst: bst.h node.o 
	$(CC) bst.o node.o -o bst

bst.o: bst.h node.c
	$(CC) $(CFLAGS) -c bst.c node.c

node: node.o node.h
	$(CC) node.o -o node

node.o: node.c node.h
	$(CC) $(CFLAGS) -c node.c

bf: bf.o bf.h bv.o speck.o
	$(CC) bf.o bv.o speck.o -o bf

bf.o: bf.c bf.h bv.c speck.c
	$(CC) $(CFLAGS) -c bf.c bv.c speck.c

bv: bv.o bv.h
	$(CC) bv.o -o bv

bv.o: bv.c bv.h
	$(CC) $(CFLAGS) -c bv.c

banhammer: parser.o node.o bf.h bv.o bst.o ht.o speck.o banhammer.o
	$(CC)  parser.o banhammer.o node.o bst.o ht.o speck.o bv.o -g -lm -o banhammer

banhammer.o: bf.h node.c bv.c parser.c bst.c ht.c speck.c banhammer.c
	$(CC) $(CFLAGS) -g -c banhammer.c parser.c node.c bst.c bv.c ht.c speck.c

format:
	clang-format -i -style=file *.[ch]

clean:
	rm -f banhammer parser.o banhammer.o bv.o bf.o bst.o node.o ht.o speck.o
