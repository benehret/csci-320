#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

/*
This function returns a pointer to an array with all the numbers in the file as int64_t
Sets *size to be the number of numbers
*/
int64_t* Populate(char* name, uint64_t* size){
    FILE* file = fopen(name, "r");
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

/*
TODO: Partitions the array and returns the index of the pivot (after partition)
*/
uint64_t partition(int64_t* input, uint64_t size){
    int64_t pivot = input[size - 1];
    uint64_t i = 0;
    for (uint64_t j = 0; j < size - 1; ++j) {
        if (input[j] < pivot) {
            int64_t temp = input[i];
            input[i] = input[j];
            input[j] = temp;
            i++;
        }
    }

    int64_t temp = input[i];
    input[i] = input[size - 1];
    input[size - 1] = temp;

    return i;
}

/*
Sorts the input array and puts output back into the input array
*/
int my_qsort(int64_t* input, uint64_t size){
    if (size < 2) return 0; // Base case

    uint64_t p = partition(input, size);

    // Recursively sort elements before partition and after partition
    if (p > 0) my_qsort(input, p); // Sort elements before pivot
    my_qsort(input + p + 1, size - p - 1); // Sort elements after pivot

    return 0;
}

/*
Returns 0 if not sorted, returns 1 if sorted
TODO: Modify if necessary
*/
int is_sorted(int64_t* input, uint64_t size){
	for (uint64_t i = 1; i < size; i++){
		if (input[i-1] > input[i]){
			return 0;
		}	
	}
    return 1;
}

int main(int argc, char** argv){
    uint64_t n; //The input size
    int64_t* input = Populate("./numbers.txt", &n); //gets the array

    
    printf("Array elements:\n");
    for (uint64_t i = 0; i < n; i++) {
        printf("%" PRId64 "\n", input[i]);
    }
    printf("\n\n\n");
    
    my_qsort(input, n);
    
    printf("Array elements:\n");
    for (uint64_t i = 0; i < n; i++) {
        printf("%" PRId64 "\n", input[i]);
    }
    printf("\n\n\n");
    
    
    //check if it's sorted.
    int sorted = is_sorted(input, n);
    printf("Are the numbers sorted? %s \n", sorted ? "true" : "false");
   
    printf("Time elapsed: %lf \n", 0.0);
    free(input);
}