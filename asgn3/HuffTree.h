#ifndef HuffTree
#define HuffTree

#define NUMCHAR 256
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <arpa/inet.h>

typedef struct HuffNode{
    char chr;
    int freq;
    struct HuffNode *left;
    struct HuffNode *right;
    int isLeaf;
} HuffNode;

/*Huffman Tree Functions*/
HuffNode *createHuffNode(char, int);
HuffNode **createHuffNodeArray();
void put(HuffNode **, char, int *);
HuffNode **cleanNodeArr(HuffNode **, int);
HuffNode **buildHuffTree(HuffNode **, int);
int compare(const void *, const void *);
/*Encoding*/
void writeHeader(HuffNode *, int, unsigned char);
int charcmp(const void *, const void *);
void fillArr(HuffNode **, HuffNode *);
void writeCodes(HuffNode *, int, int);
void setCodes(HuffNode *, char **, char *, unsigned char *);
/*Decoding*/
void decode(HuffNode *, int, int, int);
/*Free Functions*/
void freeNodeArr(HuffNode **);
void freeTree(HuffNode *);
#endif

