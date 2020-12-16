#ifndef Hashtable
#define Hashtable

#include "linkedlist.h"

#define TABLESIZE 1024
#include <stdlib.h>
#include <string.h>

typedef struct HashTable{
    int numitems;
    Node *table[TABLESIZE];
    int size;
} HashTable;

HashTable *createHashTable();
void freeHashTable(HashTable *);
int hash_string(char *, int);
void put(HashTable *, char *, int);
int get(HashTable *, char *);
Pair *getTopK(HashTable *, int);
void freeAllPairs(Pair *);
#endif