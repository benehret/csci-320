#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <inttypes.h>

/*
This function returns a pointer to an array with all the numbers in the file as int64_t
Sets *size to be the number of numbers
*/

int64_t* Populate(uint64_t* size){
    FILE* file = fopen("numbers.txt", "r");
    if (file == NULL) {
        printf("Error opening file\n");
        exit(1);
    }

    fscanf(file, "%" PRIu64, size); // Read the number of elements

    int64_t* array = malloc(*size * sizeof(int64_t));
    if (array == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    // Read the numbers into the array
    for (uint64_t i = 0; i < *size; i++) {
        fscanf(file, "%" PRId64, &array[i]);
    }

    fclose(file);
    return array;
}


int my_sort(int64_t* input, uint64_t size){
    // Bubble Sort implementation
    for (uint64_t i = 0; i < size - 1; i++) {
        for (uint64_t j = 0; j < size - i - 1; j++) {
            if (input[j] > input[j + 1]) {
                // Swap elements
                int64_t temp = input[j];
                input[j] = input[j + 1];
                input[j + 1] = temp;
            }
        }
    }
    return 0;
}

/*
Suggested function to write, to check whether the array is sorted
Returns 0 if not sorted, returns 1 if sorted
*/
int is_sorted(int64_t* input, uint64_t size){
    // Check if the array is sorted
    for (uint64_t i = 0; i < size - 1; i++) {
        if (input[i] > input[i + 1]) {
            return 0; // Not sorted
        }
    }
    return 1; // Sorted
}

int main(int argc, char** argv){

    struct timespec start, end; //structs used for timing purposes, it has two memebers, a tv_sec which is the current second, and the tv_nsec which is the current nanosecond.
    double time_diff;

    uint64_t n; // The input size
    int64_t* input = Populate(&n); // Read the numbers from the file

    clock_gettime(CLOCK_MONOTONIC, &start); //Start the clock!
	
    my_sort(input, n); // Sort the array

    clock_gettime(CLOCK_MONOTONIC, &end);   //Stops the clock!


    // Check if it's sorted
    int sorted = is_sorted(input, n);
    printf("Are the numbers sorted? %s \n", sorted ? "true" : "false");

    time_diff = (end.tv_sec - start.tv_sec); //Difference in seconds
    time_diff += (end.tv_nsec - start.tv_nsec) / 1e9; //Difference in nanoseconds

    printf("Time elapsed: %lf seconds\n", time_diff);

    free(input); // Free allocated memory

    return 0;
}
