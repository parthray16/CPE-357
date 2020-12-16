#include "hashtable.h"
#include "safe_mem_functions.h"

HashTable *createHashTable(){
    int i = 0;
    HashTable *hashmap = NULL;
    hashmap = (HashTable *)safe_malloc(sizeof(HashTable));
    hashmap->numitems = 0;
    hashmap->size = TABLESIZE;
    for(i = 0; i < hashmap->size; i++)
        hashmap->table[i] = NULL;
    return hashmap; 
}

void freeHashTable(HashTable *hashmap){
    Node *head = NULL;
    int i = 0;
    for(i = 0; i < hashmap->size; i++){
        if((head = hashmap->table[i]) != NULL){
            freeLL(head);
        }
    }
    free(hashmap);
}

int hash_string(char *key, int size){
    int hash_idx = 0;
    while (*key != '\0'){
        hash_idx = (hash_idx * 31 + *key) % size;
        key++;
    }
    return hash_idx;
}

void put(HashTable *hashtable, char *key, int freq){
    int idx = 0;
    Node *item = NULL;
    Pair *pair = NULL;
    Node *addNode = NULL;

    idx = hash_string(key, hashtable->size);
    item = hashtable->table[idx];
    if (item == NULL){
        pair = createPair(key, freq);
        hashtable->table[idx] = createNode(pair);
        hashtable->numitems++;
        return;
    }
    else{
        while (item){
            if (strcmp(item->data->key, key) == 0){
                item->data->freq++;
                free(key);
                return;
            }
            item = item->next;
        }
        pair = createPair(key, freq);
        addNode = createNode(pair);
        addNode->next = hashtable->table[idx];
        hashtable->table[idx] = addNode;
        hashtable->numitems++;
        return;
    }
}

int get(HashTable *hashtable, char *key){
    int idx = 0;
    Node *item = NULL;

    idx = hash_string(key, hashtable->size);
    item = hashtable->table[idx];
    while(item){
        if (strcmp(item->data->key, key) == 0){
            return item->data->freq;
        }
        item = item->next;
    }
    return -1;
}

Pair *getTopK(HashTable *hashtable, int k){
    Node *head = NULL;
    Pair *allPairs = NULL;
    Pair temp;
    Pair max;
    int max_idx = 0;
    int i = 0;
    int j = 0;

    allPairs = (Pair *)safe_malloc(hashtable->numitems * sizeof(Pair));
    while (i < hashtable->size && j < hashtable->numitems){
        if(hashtable->table[i]){
            head = hashtable->table[i];
            while (head){
                allPairs[j++] = *(head->data);
                head = head->next;
            }
        }
        i++;
    }
    i = 0;
    while(i < k){
        max = allPairs[i];
        max_idx = i;
        for (j = i; j < hashtable->numitems; j++){
            if(allPairs[j].freq > max.freq){
                max = allPairs[j];
                max_idx = j;
            }
            else if (allPairs[j].freq == max.freq){
                if (strcmp(allPairs[j].key, max.key) > 0){
                    max = allPairs[j];
                    max_idx = j;
                }
            }
        }
        temp = allPairs[i];
        allPairs[i] = max;
        allPairs[max_idx] = temp;
        i++;
    }
    return allPairs;
}

void freeAllPairs(Pair *allPairs){
    free(allPairs);
}
