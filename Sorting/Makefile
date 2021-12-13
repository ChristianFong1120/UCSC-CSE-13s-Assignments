CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra
all : sorting

insert: insert.o stats.o insert.h
	$(CC) insert.o stats.o -o insert

insert.o: insert.c stats.c insert.h
	$(CC) $(CFLAGS) -c insert.c stats.c

heap: heap.o heap.h
	$(CC) heap.o -o 

heap.o: heap.c heap.h
	$(CC) $(CFLAGS) -c heap.c

quick: quick.o quick.h
	$(CC) quick.o -o quick

quick.o: quick.c quick.h
	$(CC) $(CFLAGS) -c quick.c

stats: stats.o stats.h
	$(CC) stats.o -o stats

stats.o: stats.c stats.h
	$(CC) $(CFLAGS) -c stats.c

shell: shell.o shell.h
	$(CC) shell.o -g  -o shell

shell.o: shell.c shell.c
	$(CC) $(CFLAGS) -c -g shell.c 

sorting: sorting.o insert.o stats.o shell.o heap.o quick.o
	$(CC) sorting.o insert.o stats.o shell.o heap.o quick.o -g -lm -o sorting

sorting.o: sorting.c insert.c stats.c shell.c heap.c quick.c
	$(CC) $(CFLAGS) -c -g sorting.c insert.c shell.c stats.c heap.c quick.c
format:
	clang-format -i -style=file *.[ch]
clean:
	rm -f sorting sorting.o heap.o insert.o quick.o stats.o shell.o 

