#include "HuffTree.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    FILE *file = NULL;
    char c = 0;
    HuffNode **nodeArr = NULL;
    int numitems = 0;

    if (argc != 2){
        perror("Invalid Input");
        exit(EXIT_FAILURE);
    }
    if ((file = fopen(argv[1], "r")) == NULL){
        perror(argv[1]);
        exit(EXIT_FAILURE);
    }
    nodeArr = createHuffNodeArray();
    while (1){
        c = fgetc(file);
        if (feof(file)){
            break;
        }
        put(nodeArr, c, &numitems);
    }
    fclose(file);
    nodeArr = cleanNodeArr(nodeArr, numitems);
    nodeArr = buildHuffTree(nodeArr, numitems);
    printCodes(nodeArr[0]);
    freeNodeArr(nodeArr);
    return 0;
}