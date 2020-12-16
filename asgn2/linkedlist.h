#ifndef LinkedList
#define LinkedList

#include <stdlib.h>

typedef struct Pair {
    char *key;
    int freq;
} Pair;

typedef struct Node {
    Pair *data;
    struct Node *next;
} Node;

Node *createNode(Pair *data);
void freeLL(Node *);
Pair *createPair(char *key, int);
void freePair(Pair *);
#endif