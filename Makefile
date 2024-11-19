CC = gcc
CFLAGS = -pthread -Wall -Wextra

all: process_threads

process_threads: jonessn_HW04.c
	$(CC) $(CFLAGS) -o process_threads jonessn_HW04.c
clean:
	rm -f process_threads