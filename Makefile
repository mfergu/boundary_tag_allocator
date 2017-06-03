CC=clang
CFLAGS= -Weverything -g 
BINS=test

all: $(BINS)

test: pfl1.c test1.c
	$(CC) $(CFLAGS) -o test pfl1.c test1.c
