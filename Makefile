flags=

all: main.o board.o piece.o fitness.o algorithm.o reader.o
	gcc -Wall main.o board.o piece.o fitness.o algorithm.o reader.o -o main $(flags)

main.o: main.c board.h piece.h fitness.h reader.h algorithm.h
	gcc -Wall -c main.c $(flags)

piece.o: piece.c piece.h
	gcc -Wall -c piece.c $(flags)

board.o: board.c board.h piece.h fitness.h
	gcc -Wall -c board.c $(flags)

fitness.o: fitness.c fitness.h board.h piece.h reader.h
	gcc -Wall -c fitness.c $(flags)

reader.o: reader.c reader.h algorithm.h
	gcc -Wall -c reader.c $(flags)

algorithm.o: algorithm.c algorithm.h fitness.h reader.h
	gcc -Wall -c algorithm.c $(flags)

clean:
	rm *.o

purge:
	rm *.o main

debug: flags=-g
debug: all

opt: flags=-O3
opt: all