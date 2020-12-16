#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <string.h>
#include <ctype.h>

int checkint(char *);

void handler(int signum){
    static int t = 0;
    if (t++ % 2){
        if (write(STDERR_FILENO, "tock.\n", sizeof(char) * strlen("tock.\n")) == -1){
            perror("write error");
            exit(EXIT_FAILURE);
        }
    }
    else{
        if (write(STDERR_FILENO, "Tick...", sizeof(char) * strlen("Tick...")) == -1){
            perror("write error");
            exit(EXIT_FAILURE);
        }
    }    
}

int main(int argc, char *argv[]){
    struct sigaction sa;
    struct itimerval it;
    unsigned int n;
    unsigned int i = 0;

    if (argc != 2){
        //print usage error
        printf("usage: timeit <seconds>\n");
        exit(EXIT_FAILURE);
    }
    if (checkint(argv[1]) == 0){
        //print usage error
        printf("%s: malformed time.\n", argv[1]);
        printf("usage: timeit <seconds>\n");
        exit(EXIT_FAILURE);
    }
    n = strtol(argv[1], &argv[1], 10);
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGALRM, &sa, NULL) == -1){
        perror("sigerror");
        exit(EXIT_FAILURE);
    }
    
    it.it_interval.tv_sec = 0;
    it.it_interval.tv_usec = 500000;
    it.it_value.tv_sec = 0;
    it.it_value.tv_usec = 500000;

    if (setitimer(ITIMER_REAL, &it, NULL) == -1){
        perror("settimer");
        exit(EXIT_FAILURE);
    }
    for(i = 0; i < n * 2; i++){
        pause();
    }
    if (write(STDERR_FILENO, "Time's up!\n", sizeof(char) * strlen("Time's up!\n")) == -1){
            perror("write error");
            exit(EXIT_FAILURE);
    }
    return 0;
}

int checkint(char *arg){
    int i = 0;
    for (i = 0; i < strlen(arg); i++){
        if(isdigit(arg[i]) == 0){
            return 0;
        }
    }
    return 1;
}