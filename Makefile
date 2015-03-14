CC=gcc
CFLAGS=-Wall

all:
	$(CC) $(CFLAGS) main.c -o elevator
clean:
	rm -rf *.o elevator
