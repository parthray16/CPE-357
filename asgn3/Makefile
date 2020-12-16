all: hencode hdecode

hencode: hencode.o HuffTree.o safe_mem_functions.o
	gcc -Wall -pedantic -g -O  -o hencode hencode.o HuffTree.o safe_mem_functions.o

hdecode: hdecode.o HuffTree.o safe_mem_functions.o
	gcc -Wall -pedantic -g -O  -o hdecode hdecode.o HuffTree.o safe_mem_functions.o

hencode.o: hencode.c HuffTree.h
	gcc -Wall -pedantic -g -O -c hencode.c

hdecode.o: hdecode.c HuffTree.h
	gcc -Wall -pedantic -g -O -c hdecode.c

HuffTree.o: HuffTree.c HuffTree.h
	gcc -Wall -pedantic -g -O -c HuffTree.c

safe_mem_functions.o: safe_mem_functions.c safe_mem_functions.h
	gcc -Wall -pedantic -g -O -c safe_mem_functions.c

clean:
	rm hencode hdecode hencode.o hdecode.o HuffTree.o safe_mem_functions.o