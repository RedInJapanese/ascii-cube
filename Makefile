CC=clang
CFLAGS=-Wall -Werror -Wextra -Wpedantic

all: cube
	$(CC) -o cube cube.o
cube.o:
	$(CC) $(CLFAGS) -c cube.c

clean: 
	rm -rf cube cube.o
