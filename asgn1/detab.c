#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    int c;
    int numspaces = 0;
    int pos = 0;

    while ((c = getchar()) != EOF){
        if (c == '\t'){
            numspaces = 8 - (pos % 8);
            while (numspaces > 0){
                putchar(' ');
                numspaces--;
                pos++;
            }
        }
        else if ((c == '\n') || (c == '\r')){
            putchar(c);
            pos = 0;
        }
        else if (c == '\b'){
            if (pos != 0){
                pos--;
            }
            putchar(c);
        }
        else{
            putchar(c);
            pos++;
        }
    }
    return 0;
}
