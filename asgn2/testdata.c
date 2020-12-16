#include "hashtable.h"
#include "linkedlist.h"
#include "safe_mem_functions.h"

#include <stdio.h>

int main(int argc, char *argv[]){
    HashTable *hashmap = NULL;
    Pair *top5 = NULL;
    char *c1 = "parth";
    char *c2 = "arth";
    char *c3 = "rth";
    char *c4 = "th";
    char *c5 = "parth";
    char *c6 = "adam";
    hashmap = createHashTable();
    printf("%d\n", hashmap->numitems);
    put(hashmap, c1, 1);
    put(hashmap, c2, 1);
    put(hashmap, c3, 1);
    put(hashmap, c4, 1);
    put(hashmap, c5, 1);
    put(hashmap, c6, 1);
    printf("Size: %d\n", hashmap->size);
    printf("%d\n", hashmap->numitems);
    printf("%d\n", get(hashmap, c2));
    printf("%d\n", get(hashmap, c5));
    printf("%d\n", get(hashmap, c6));
    top5 = getTopK(hashmap, 5);
    printf("%c\n", *(top5[0].key));
    printf("%c\n", *(top5[1].key));
    printf("%c\n", *(top5[2].key));
    printf("%c\n", *(top5[3].key));
    printf("%c\n", *(top5[4].key));
    freeAllPairs(top5);
    freeHashTable(hashmap);
    return 0;
}