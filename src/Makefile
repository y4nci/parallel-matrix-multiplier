CC=g++
CFLAGS=-ansi -Wall -pedantic-errors -O0 -std=c++11

all: hw2

tar: hw2.cpp Inputs.cpp Inputs.h Matrix.cpp Matrix.h hw2_output.c Semaphore.cpp Semaphore.h Makefile
	tar -cvf hw2.tar hw2.cpp Inputs.cpp Inputs.h Matrix.cpp Matrix.h hw2_output.c Makefile
hw2: hw2.cpp Inputs.cpp Matrix.cpp hw2_output.c Semaphore.cpp Semaphore.h
	$(CC)	$(CFLAGS)	hw2.cpp	Inputs.cpp	Matrix.cpp	Semaphore.cpp	hw2_output.c	-o	hw2

clean:
	rm -f hw2
