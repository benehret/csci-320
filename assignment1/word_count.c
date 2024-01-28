#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 10

int main(int argc, char** argv){

    char buffer[BUFFER_SIZE];

    FILE* file = fopen("hello.txt", "r");
    fgets(buffer, sizeof(buffer), file);
    fclose(file);

    printf("Number of args: %d\n", argc);
    int i = 0;
    while (i < argc){
        printf("Arg %d: %s\n", i, argv[i]);
        i++;
    }

    printf("Text: %s\n", buffer);
    return (intptr_t) &main;
}