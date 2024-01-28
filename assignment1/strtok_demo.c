#include <stdio.h>
#include <string.h>

int main(int argc, char** argv){
    char x[] = "turkey trots to water gg from cincpac action com third fleet info";
    printf("%s\n", x);

    //strtok with the string x with <space> as the delimiter
    char* tok = strtok(x, " ");
    printf("%s\n", tok);

    //Subsequent calls with NULL as input will pick up where the previous left off
    tok = strtok(NULL, " ");
    printf("%s\n", tok);
    tok = strtok(NULL, " ");
    printf("%s\n", tok);

    //Notice that strtok is destructive, x does not have the full string any more
    printf("%s\n", x);
}