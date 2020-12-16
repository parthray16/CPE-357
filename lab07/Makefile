all: forkit tryit

forkit: forkit.o
	gcc -Wall -g -o forkit forkit.o

tryit: tryit.o 
	gcc -Wall -g -o tryit tryit.o

forkit.o: forkit.c
	gcc -Wall -g -c forkit.c

tryit.o: tryit.c
	gcc -Wall -g -c tryit.c

clean:
	rm forkit tryit forkit.o tryit.o