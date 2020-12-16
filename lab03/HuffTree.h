#ifndef HuffTree
#define HuffTree

#define NUMCHAR 256
#include <stdlib.h>
#include <string.h>

typedef struct HuffNode{
    char chr;
    int freq;
    struct HuffNode *left;
    struct HuffNode *right;
    int isLeaf; //1 true
} HuffNode;

HuffNode *createHuffNode(char, int);
HuffNode **createHuffNodeArray();
void put(HuffNode **, char, int *);
HuffNode **cleanNodeArr(HuffNode **, int);
HuffNode **buildHuffTree(HuffNode **, int);
int compare(const void *, const void *);
void printCodes(HuffNode *);
void setCodes(HuffNode *, char **, char *);
void freeNodeArr(HuffNode **);
void freeTree(HuffNode *);
#endif

