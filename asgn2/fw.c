#include "readword.h"
#include "hashtable.h"

int main(int argc, char *argv[]){
    HashTable *hashtable = NULL;
    char *word = NULL;
    int k = 10;
    FILE *file = NULL;
    int i = 0;
    char *flag = "-n";
    Pair *topK = NULL;
    int fileGiven = 0; //false

    hashtable = createHashTable();
    
    for (i = 1; i < argc; i++){
        if (i == 1 && (strcmp(flag, argv[1])) == 0){
            i = 2;
            if ((i < argc) && isdigit(argv[i][0])){
                k = atoi(argv[i]);
            }
            else{
                printf("usage: fw [-n num] [ file1 [ file 2 ...] ]\n");
                return 0;
            }
        }
        else if((file = fopen(argv[i], "r")) == NULL){
            if ((i + 1) >= argc){
                fileGiven = 1;
            }
            perror(argv[i]);
            continue;
        }
        else{
            fileGiven = 1;  //true
            while((word = readword(file))){
                if (*word == '\0'){
                    free(word);
                    continue;
                }
                put(hashtable, word, 1);
            }
            fclose(file);
        }
    }
    if (!(fileGiven)){
        while((word = readword(stdin))){
            if (*word == '\0'){
                free(word);
                continue;
            }
            put(hashtable, word, 1);
        }
    }    
    i = 0;  
    topK = getTopK(hashtable, k);
    printf("The top %d words (out of %d) are:\n", k, hashtable->numitems);
    while(i < k && (i < hashtable->numitems) && (topK[i].key)){
        printf("%9d %s\n", topK[i].freq, topK[i].key);
        i++;
    }
    freeAllPairs(topK);
    freeHashTable(hashtable);
    return 0;
}