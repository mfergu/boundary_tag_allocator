CC=clang
CFLAGS=-std=c11 -Weverything -g 
BINS=temp_compile

all: $(BINS)

temp_compile: pfl1.c
	$(CC) $(CFLAGS) -c -o temp_compile pfl1.c
