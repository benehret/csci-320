#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <omp.h>

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

void merge(int64_t* input, int64_t* temp, uint64_t left, uint64_t mid, uint64_t right) {
    uint64_t i = left, j = mid + 1, k = left;
    while (i <= mid && j <= right) {
        if (input[i] < input[j]) {
            temp[k++] = input[i++];
        } else {
            temp[k++] = input[j++];
        }
    }
    while (i <= mid) {
        temp[k++] = input[i++];
    }
    while (j <= right) {
        temp[k++] = input[j++];
    }
    for (i = left; i <= right; i++) {
        input[i] = temp[i];
    }
}

/*
Helper for msort
*/
int my_msort_helper(int64_t* input, int64_t* temp, uint64_t size) {
    if (size < 2) return 0;

    uint64_t mid = size / 2;
    #pragma omp task firstprivate(size) if (size > 1000)
    my_msort_helper(input, temp, mid);
    #pragma omp task firstprivate(size) if (size > 1000)
    my_msort_helper(input + mid, temp + mid, size - mid);
    #pragma omp taskwait 
    merge(input, temp, 0, mid - 1, size - 1);
    return 0;
}

/*
Sorts the input array and puts output back into the input array
*/
int my_msort(int64_t* input, uint64_t size){
    int64_t* temp = malloc(size * sizeof(int64_t)); //Only create 1 temporary array
    #pragma omp parallel
    {
        #pragma omp single
        {
            my_msort_helper(input, temp, size); //Calls helper
        }
    }
    

    free(temp);
    return 0;
}

/*
Suggested function to write, to check whether the array is sorted
Returns 0 if not sorted, returns 1 if sorted
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
    struct timespec start, end; //structs used for timing purposes, it has two memebers, a tv_sec which is the current second, and the tv_nsec which is the current nanosecond.
    double time_diff;
    uint64_t n; //The input size
    int64_t* input = Populate("./numbers.txt", &n); //gets the array
    clock_gettime(CLOCK_MONOTONIC, &start); //Start the clock!
    my_msort(input, n);
    clock_gettime(CLOCK_MONOTONIC, &end);   //Stops the clock!
    //check if it's sorted.
    int sorted = is_sorted(input, n);
    printf("Are the numbers sorted? %s \n", sorted ? "true" : "false");
   
    time_diff = (end.tv_sec - start.tv_sec); //Difference in seconds
    time_diff += (end.tv_nsec - start.tv_nsec) / 1e9; //Difference in nanoseconds

    printf("Time elapsed: %lf seconds\n", time_diff);
    free(input);
}
