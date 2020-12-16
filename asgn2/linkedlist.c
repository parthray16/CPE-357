#include "linkedlist.h"
#include "safe_mem_functions.h"

Node *createNode(Pair *data){
    Node *head = NULL;
    head = (Node *)safe_malloc(sizeof(Node));
    head->data = data;
    head->next = NULL;
    return head;
}

void freeLL(Node *head) {
    Node *temp = NULL;
    while (head != NULL){
        freePair(head->data);
        temp = head;
        head = head->next;
        free(temp);
    }
}

Pair *createPair(char *key, int freq){
    Pair *pair = NULL;
    pair = (Pair *)safe_malloc(sizeof(Pair));
    pair->key = key;
    pair->freq = freq;
    return pair;
}

void freePair(Pair *pair){
    free(pair->key);
    free(pair);
}