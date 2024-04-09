#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <inttypes.h>

int generate(int64_t array[], size_t size){
    srand(time(NULL));
    for (size_t i = 0; i < size; i++){
        array[i] = rand()%(4*size);
    }
    return 0;
}

/*
Generates a BUNCH of random numbers depending on the command line argument and puts it into a file
(By default just 10). 

Note that the first line of the file is the count of how many numbers there will be.
*/
int main(int argc, char** argv){
    int64_t n = 10;
    if (argc==2){
        n = strtoull(argv[1], NULL, 10);
    }

    srand(time(NULL));
    FILE* fptr = fopen("./numbers.txt", "w+");
    fprintf(fptr, "%" PRIu64 "\n\n", n);
    for (int64_t i = 0; i < n; i++){
        fprintf(fptr, "%" PRIu64 "\n", rand()%(4*n));
    }
    fclose(fptr);

    return 0;
}