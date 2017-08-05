.PHONY: minesweeper clean
CC=clang++
CFLAGS=-I. -std=c++11
minesweeper:
	$(CC) -g -o minesweeper main.cpp field.cpp $(CFLAGS)

clean:
	rm -f minesweeper
