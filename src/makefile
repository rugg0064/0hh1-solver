CC=gcc
CFLAGS=-I. -Wall
DEPS = constraints.h board.h uints.h solver.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

mainProgram: board.o constraints.o main.o solver.o uints.o
	$(CC) -o a.out board.o constraints.o main.o solver.o uints.o
	time ./a.out
	
testUint: uints.o ./tests/testUint.c
	$(CC) -o a.out uints.o ./tests/testUint.c
	./a.out