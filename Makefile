export LANG=en
CC=gcc
CFLAGS=-c -Wall -g -O3 -std=c1x
INCLUDE=-mwindows -Wl,-subsystem,windows

EXEC=output
SRC=$(wildcard src/**/*.c) $(wildcard src/*.c)
OBJ=$(SRC:.c=.o)

all: $(SRC) $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) $(INCLUDE) -o $@

.c.o:
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@

clean:
	rm -f $(OBJ)
	rm -f $(EXEC)

run: all
	./$(EXEC)




