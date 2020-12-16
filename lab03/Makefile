all: htable.c
		gcc -Wall -g -o htable htable.c HuffTree.c safe_mem_functions.c
htable: htable.c
		gcc -Wall -g -o htable htable.c HuffTree.c safe_mem_functions.c
clean:
		rm htable
test: htable
		echo Testing htable
		./htable test
		echo done.