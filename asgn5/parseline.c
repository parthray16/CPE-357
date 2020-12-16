#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#define CMDLINELENGTH 512
#define CMDPIPELIM 10
#define ARGSLIM 10

void print_stage(char *line, int stage, int in, int out){
    int inrd = 0;
    int outrd = 0;
    int i = 0;
    char copy[CMDLINELENGTH];
    char indest[CMDLINELENGTH] = "original stdin";
    char outdest[CMDLINELENGTH] = "original stdout";
    char *rest = line;
    char *cmd = NULL;
    char *token = NULL;
    int argc = 0;
    char *argv[10];
    strcpy(copy, line);
    cmd = strtok_r(rest, " ", &rest);
    /*check cmd*/
    if ((strcmp(cmd, "<") == 0) || (strcmp(cmd, ">") == 0)){
        fprintf(stderr, "%s: not a valid command\n", cmd);
        exit(EXIT_FAILURE);
    }
    argc++;
    argv[0] = strdup(cmd);
    for(i = 0; copy[i] != '\0'; i++){
        if(copy[i] == '<'){
            inrd++;
        }
        if(copy[i] == '>'){
            outrd++;
        }
    }
    /*multiple redirections*/
    if(inrd > 1){
        fprintf(stderr, "%s: bad input redirection\n", cmd);
        exit(EXIT_FAILURE);
    }
    if(outrd > 1){
        fprintf(stderr, "%s: bad output redirection\n", cmd);
        exit(EXIT_FAILURE);
    }
    /*4 cases*/
    /*First Stage*/
    if(in == -1 && out == -1){
        printf("\n--------\nStage %d: \"%s\"\n--------\n", stage, copy);
        while((token = strtok_r(rest, " ", &rest))){
            if(strcmp(token, "<") == 0){
                token = strtok_r(rest, " ", &rest);
                if(token == NULL || strcmp(token, ">") == 0 
                                 || strcmp(token, "") == 0){
                    fprintf(stderr, "%s: bad input redirection\n", cmd);
                    exit(EXIT_FAILURE);
                }
                strcpy(indest, token);
            }
            else if(strcmp(token, ">") == 0){
                token = strtok_r(rest, " ", &rest);
                if(token == NULL || strcmp(token, "<") == 0 
                                 || strcmp(token, "") == 0){
                    fprintf(stderr, "%s: bad output redirection\n", cmd);
                    exit(EXIT_FAILURE);
                }
                strcpy(outdest, token);
            }
            else{
                argc++;
                if(argc > 10){
                    fprintf(stderr, "%s: too many arguments\n", cmd);
                    exit(EXIT_FAILURE);
                }
                argv[argc - 1] = strdup(token);
            }
        }
        printf("input: %s\n", indest);
        printf("output: %s\n", outdest);
    }
    /*cannot have output redirection '>' if out is pipe*/
    if(in == -1 && out != -1){
        if(outrd > 0){
            fprintf(stderr, "%s: ambiguous output\n", cmd);
            exit(EXIT_FAILURE);
        }
        printf("\n--------\nStage %d: \"%s\"\n--------\n", stage, copy);
        while((token = strtok_r(rest, " ", &rest))){
            if(strcmp(token, "<") == 0){
                token = strtok_r(rest, " ", &rest);
                if(token == NULL || strcmp(token, ">") == 0 
                                 || strcmp(token, "") == 0){
                    fprintf(stderr, "%s: bad input redirection\n", cmd);
                    exit(EXIT_FAILURE);
                }
                strcpy(indest, token);
            }
            else{
                argc++;
                if(argc > 10){
                    fprintf(stderr, "%s: too many arguments\n", cmd);
                    exit(EXIT_FAILURE);
                }
                argv[argc - 1] = strdup(token);
            }
        }
        printf("input: %s\n", indest);
        printf("output: pipe to stage %d\n", out);
    }

    /*Middle Stage*/
    /*cannot have any redirection*/
    if(in != -1 && out != -1){
        if(inrd > 0){
            fprintf(stderr, "%s: ambiguous input\n", cmd);
            exit(EXIT_FAILURE);
        }
        if(outrd > 0){
            fprintf(stderr, "%s: ambiguous output\n", cmd);
            exit(EXIT_FAILURE);
        }
        printf("\n--------\nStage %d: \"%s\"\n--------\n", stage, copy);
        while((token = strtok_r(rest, " ", &rest))){
            argc++;
            if(argc > 10){
                fprintf(stderr, "%s: too many arguments\n", cmd);
                exit(EXIT_FAILURE);
            }
            argv[argc - 1] = strdup(token);
        }
        printf("input: pipe from stage %d\n", in);
        printf("output: pipe to stage %d\n", out);
    }
    /*Last Stage*/
    /*cannot have input redirection '<' if in is pipe*/
    if(in != -1 && out == -1){
        if(inrd > 0){
            fprintf(stderr, "%s: ambiguous input\n", cmd);
            exit(EXIT_FAILURE);
        }
        printf("\n--------\nStage %d: \"%s\"\n--------\n", stage, copy);
        while((token = strtok_r(rest, " ", &rest))){
            if(strcmp(token, ">") == 0){
                token = strtok_r(rest, " ", &rest);
                if(token == NULL || strcmp(token, "<") == 0 
                                 || strcmp(token, "") == 0){
                    fprintf(stderr, "%s: bad output redirection\n", cmd);
                    exit(EXIT_FAILURE);
                }
                strcpy(outdest, token);
            }
            else{
                argc++;
                if(argc > 10){
                    fprintf(stderr, "%s: too many arguments\n", cmd);
                    exit(EXIT_FAILURE);
                }
                argv[argc - 1] = strdup(token);
            }
        }
        printf("input: pipe from stage %d\n", in);
        printf("output: %s\n", outdest);
    }
    printf("argc: %d\n", argc);
    printf("argv: ");
    for(i = 0; i < argc; i++){
        if (i == argc - 1){
            printf("\"%s\"\n", argv[i]);
        }
        else{
            printf("\"%s\",", argv[i]);
        }
       free(argv[i]);
    }
}
int main(int argc, char *argv[]){
    char line[CMDLINELENGTH * 2];
    int i = 0;
    char *token = NULL;
    int stage = 0;
    int totalPipes = 0;
    int isPipe = 0;
    if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO)){
        printf("line: ");
        fflush(stdout);
    }
    if (fgets(line, CMDLINELENGTH * 2, stdin)){
        if(strlen(line) > CMDLINELENGTH){
            fprintf(stderr, "command too long\n");
            exit(EXIT_FAILURE);
        }
        for(i = 0; line[i] != '\0'; i++){
            if(line[i] == '|'){
                totalPipes++;
            }
        }
        if(totalPipes >= CMDPIPELIM){
            fprintf(stderr, "pipeline too deep\n");
            exit(EXIT_FAILURE);
        }
        token = strtok(line, "|\n");
        /*remove space at end*/
        if(isspace(token[strlen(token) - 1])){
            token[strlen(token) - 1] = '\0';
        }
        /*no pipe*/
        if (totalPipes == 0){
            print_stage(token, stage, -1, -1);
            stage++;
        }
        /*first command before pipe*/
        else{
            print_stage(token, stage, -1, stage + 1);
            stage++;
            isPipe = 1;
        }
        while((token = strtok(NULL, "|\n"))){
            /*remove space at end*/
            if(isspace(token[strlen(token) - 1])){
                token[strlen(token) - 1] = '\0';
            }
            /*remove space at front*/
            if(isspace(token[0])){
                token++;
            }
            if(strcmp(token, "") == 0){
                fprintf(stderr, "invalid null command\n");
                exit(EXIT_FAILURE);
            }
            /*last stage*/
            if(totalPipes == stage){
                print_stage(token, stage, stage - 1, -1);
                stage++;
                isPipe = 0;
            }
            /*middle stage*/
            else{
                print_stage(token, stage, stage - 1, stage + 1);
                stage++;
            }
        }
        /*no cmd after pipe*/
        if(isPipe){
            fprintf(stderr, "invalid null command\n");
            exit(EXIT_FAILURE);
        }   
    }
    return 0;
}
