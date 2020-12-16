#include "safe_mem_functions.h"
#include "readword.h"

char *readword(FILE *file){
    int c = 0;
    int idx = 0;
    int size = 0;
    char *word = NULL;

    while ((c = fgetc(file)) != EOF){
        if (idx > size - 2){
            size += WORD_SIZE;
            word = (char *)safe_realloc(word, size);
        }
        if (isalpha(c)){
            word[idx++] = tolower(c);
        }
        else{
            break;
        }
    }
    if(word){
        word[idx++] = '\0';
        word = (char *)safe_realloc(word, idx);   
    }
    return word; 
}
