CC=clang
CFLAGS= -Weverything -g 
BINS=test1 test2

all: $(BINS)

test1: pfl1.c test1.c
	$(CC) $(CFLAGS) -o test1 pfl1.c test1.c

test2: pfl2.c test2.c
	$(CC) $(CFLAGS) -o test2 pfl2.c test2.c
