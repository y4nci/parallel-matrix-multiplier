CC=g++
CFLAGS=-ansi -Wall -pedantic-errors -O0 -std=c++11
LDFLAGS=-lpthread

all: hw2

tar: hw2.cpp Inputs.cpp Inputs.h Matrix.cpp Matrix.h hw2_output.c Semaphore.cpp Semaphore.h Makefile
	tar -cvf hw2.tar.gz hw2.cpp Inputs.cpp Inputs.h Matrix.cpp Matrix.h hw2_output.c hw2_output.h Makefile Semaphore.cpp Semaphore.h
hw2: hw2.cpp Inputs.cpp Matrix.cpp hw2_output.c Semaphore.cpp Semaphore.h
	$(CC) $(CFLAGS) hw2.cpp Inputs.cpp Matrix.cpp Semaphore.cpp hw2_output.c -o hw2 $(LDFLAGS)

clean:
	rm -f hw2
