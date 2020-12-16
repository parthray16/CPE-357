#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef PATH_MAX
#define PATH_MAX 2048
#endif

int main(){
    DIR *directory = NULL;
    struct stat statb;
    struct stat statc;
    struct dirent *entry;
    long unsigned int curri = 0;
    int currd = 0;
    long unsigned int parenti = 0;
    int parentd = 0;
    char path[PATH_MAX + 1];
    int size = PATH_MAX;
    int root = 0;
    int j = 0;

    path[size] = '\0';
    while(1){
        if(lstat(".", &statb) == -1){
            perror("cannot get current directory");
            exit(EXIT_FAILURE);
        }
        curri = statb.st_ino;
        currd = statb.st_dev;
        if(chdir("..") == -1){
            perror("mypwd");
            exit(EXIT_FAILURE);
        }
        if((directory = opendir(".")) == NULL){
            perror("mypwd");
            exit(EXIT_FAILURE);
        }
        while ((entry = readdir(directory)) != NULL){
            if(lstat(entry->d_name, &statc) == -1){
                perror("mypwd");
                exit(EXIT_FAILURE);
            }
            parenti = statc.st_ino;
            parentd = statc.st_dev;
            if(parenti == curri && parentd == currd){
                if (strcmp(entry->d_name, ".") == 0){
                    root = 1;
                    break;
                }
                size -= strlen(entry->d_name) + 1;
                if(size < 0){
                    printf("path too long");
                    exit(EXIT_FAILURE);
                }
                for(j = 0; j < strlen(entry->d_name) + 1; j++){
                    if(j == 0){
                        path[size + j] = '/';
                    }
                    else{
                        path[size + j] = entry->d_name[j-1];
                    }
                }
                break;
            }    
        }
        if(closedir(directory) == -1){
            perror("mypwd");
            exit(EXIT_FAILURE);
        }
        if (root == 1){
            break;
        }
    }
    printf("%s\n", path+size);
    return 0;
}