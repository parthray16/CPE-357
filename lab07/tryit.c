#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h> 

int main(int argc, char *argv[]){
    pid_t child, done;
    int status;
    char *args[] = {argv[1], 0};
    if (argc < 2 || argc > 2){
        fprintf(stderr,
                "Usage: tryit command\n");
        exit(EXIT_FAILURE);
    }
    if ((child = fork())){
        if((done = wait(&status)) == -1){
            perror("wait");
            exit(EXIT_FAILURE);
        }
    }
    else{
        execve(argv[1], args, NULL);
        fprintf(stderr, "%s: No such file or directory\n", argv[1]);
        exit(EXIT_FAILURE);
    }
    if(WIFEXITED(status) && WEXITSTATUS(status) == 0){
        printf("Process %d succeeded.\n", done);
        exit(EXIT_SUCCESS);
    }
    else if(WIFEXITED(status)){
        printf("Process %d exited with an error value.\n", done);
        exit(EXIT_FAILURE);
    }
    else{
        printf("Process %d terminated on signal %d\n", done, WTERMSIG(status));
    }
    return 0;
}