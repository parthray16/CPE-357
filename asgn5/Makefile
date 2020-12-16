all: parseline

parseline: parseline.o
	gcc -Wall -pedantic -g -o parseline parseline.o

parseline.o: parseline.c
	gcc -Wall -pedantic -g -c parseline.c

clean:
	rm parseline parseline.o