CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

SRC = $(wildcard src/*.c)

scheme: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o scheme

clean:
	rm -f scheme