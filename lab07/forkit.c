#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h> 

int main(int argc, char *argv[]){
    pid_t child, done;
    int status;

    printf("Hello, world!\n");
    if((child = fork())){
        printf("This is the parent, pid %d.\n", getpid());
    }
    else{
        printf("This is the child, pid %d.\n", getpid());
        exit(EXIT_SUCCESS);
    }
    if((done = wait(&status)) == -1){
        perror("wait");
        exit(EXIT_FAILURE);
    }
    printf("This is the parent, pid %d, signing off.\n", getpid());
    return 0;
}