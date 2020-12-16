#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_SIZE 16

char *read_long_line(FILE *);
void *safe_malloc(size_t);
void *safe_realloc(void *, size_t);

int main(int argc, char *argv[]){
    char *curr;
    char *prev = NULL;

    curr = read_long_line(stdin);
    while (curr[0] != EOF){
        if (prev == NULL || strcmp(curr, prev)){
            puts(curr);
        }
        if (prev != NULL){
            free(prev);
        }
        prev = curr;
        curr = read_long_line(stdin);
    }
    free(curr);
    return 0;
}

char *read_long_line(FILE *file){
    char c;
    int idx = 0;
    int size = LINE_SIZE;
    char *line;

    line = (char *)safe_malloc(LINE_SIZE * sizeof(char));
    c = fgetc(file);
    while ((c != EOF) && (c != '\n')){
        if (idx == size){
            line = (char *)safe_realloc(line, 2 * size * sizeof(char));
            size *= 2;
        }
        line[idx] = c;
        idx++;
        c = fgetc(file);
    }

    if (idx == size){
        line = (char *)safe_realloc(line, 2 * size * sizeof(char));
        size *= 2;
    }
    if(c == EOF){
        line[idx] = EOF;
    }
    else{
        line[idx] = '\0';
    }
    return line; 
}

void *safe_malloc(size_t size){
    void *new;
    if (!(new = malloc(size))){
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    return new;
}

void *safe_realloc(void *ptr, size_t size){
    void *new;
    if (!(new = realloc(ptr, size))){
        perror("realloc");
        exit(EXIT_FAILURE);
    }
    return new;
}
