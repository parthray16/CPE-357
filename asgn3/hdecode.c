#include "HuffTree.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]){
    int infile = 0;
    int outfile = 1; 
    unsigned char c = 0;
    int32_t count = 0;
    HuffNode **nodeArr = NULL;
    int numitems = 0;
    int num = 0;
    int r_status = 0;
    int totalc = 0;
    

    if (argc > 3){
        perror("usage: hdecode [(infile | -) [outfile]]\n");
        exit(EXIT_FAILURE);
    }
    if (argc == 3){
        outfile = open(argv[2], O_WRONLY|O_TRUNC|O_CREAT, S_IRUSR|S_IWUSR);
        if (outfile == -1){
            perror(argv[2]);
            exit(EXIT_FAILURE);
        } 
    }
    if (argc > 1){
        if (strcmp(argv[1], "-") != 0){
            infile = open(argv[1], O_RDONLY);
            if (infile == -1){
                perror(argv[1]);
                exit(EXIT_FAILURE);
            } 
        }
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
    num = c;
    while (num >= 0){
        r_status = read(infile, &c, sizeof(char));
        if(r_status == -1){
            perror("read");
            exit(EXIT_FAILURE);
        }   
        put(nodeArr, (char)c, &numitems);
        r_status = read(infile, &count, sizeof(int32_t));
        count = ntohl(count);
        nodeArr[c]->freq = count;
        totalc += count;
        num--;
    }
    nodeArr = cleanNodeArr(nodeArr, numitems);
    nodeArr = buildHuffTree(nodeArr, numitems);
    decode(nodeArr[0], totalc, infile, outfile);
    freeNodeArr(nodeArr);

    if (infile != 0 && close(infile) == -1){
        perror(argv[1]);
        exit(EXIT_FAILURE);
    }
    if (outfile != 1 && close(outfile) == -1){
        perror(argv[2]);
        exit(EXIT_FAILURE);
    }
    return 0;
}