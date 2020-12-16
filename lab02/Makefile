all: uniq.c
		gcc -Wall -g -o uniq uniq.c
test: uniq
		echo Testing uniq
		./uniq < test1in > test1out
		echo done.
clean:
		rm uniq