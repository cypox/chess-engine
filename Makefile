IDIR =./
CC=g++
CFLAGS=-I$(IDIR)

LIBS=-lm

main: main.cpp board.cpp move.cpp piece.cpp engine.cpp
	$(CC) -g -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f main 
