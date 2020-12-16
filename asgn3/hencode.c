#include "HuffTree.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]){
    int infile = 0;
    int outfile = 1; 
    char c = 0;
    HuffNode **nodeArr = NULL;
    int numitems = 0;
    int r_status = 0;

    if (argc < 2 || argc > 3){
        perror("usage: hencode infile [outfile]\n");
        exit(EXIT_FAILURE);
    }
    if (argc == 3){
        outfile = open(argv[2], O_WRONLY|O_TRUNC|O_CREAT, S_IRUSR|S_IWUSR);
        if (outfile == -1){
            perror(argv[2]);
            exit(EXIT_FAILURE);
        } 
    }
    infile = open(argv[1], O_RDONLY);
    if (infile == -1){
        perror(argv[1]);
        exit(EXIT_FAILURE);
    }
    
    r_status = read(infile, &c, sizeof(char));
    if(r_status == -1){
        perror("read");
        exit(EXIT_FAILURE);
    }
    if (r_status == 0){
        if (close(infile) == -1){
            perror(argv[1]);
            exit(EXIT_FAILURE);
        }
        if (outfile != 1 && close(outfile) == -1){
            perror(argv[2]);
            exit(EXIT_FAILURE);
        }
        return 0;    
    }
    nodeArr = createHuffNodeArray();
    while (r_status != 0){
        if(r_status == -1){
            perror("read");
            exit(EXIT_FAILURE);
        }   
        put(nodeArr, c, &numitems);
        r_status = read(infile, &c, sizeof(char));
    }
    if (lseek(infile, 0, SEEK_SET) < 0){
        perror("seek");
        exit(EXIT_FAILURE);
    }
    nodeArr = cleanNodeArr(nodeArr, numitems);
    nodeArr = buildHuffTree(nodeArr, numitems);
    writeCodes(nodeArr[0], infile, outfile);
    freeNodeArr(nodeArr);

    if (close(infile) == -1){
        perror(argv[1]);
        exit(EXIT_FAILURE);
    }
    if (outfile != 1 && close(outfile) == -1){
        perror(argv[2]);
        exit(EXIT_FAILURE);
    }
    return 0;
}