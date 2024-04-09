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

// Implement the merge function as required by Mergesort
void merge(int64_t* arr, int64_t* temp, int left, int mid, int right) {
    int i = left;
    int j = mid + 1;
    int k = left;

    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }
    while (i <= mid) {
        temp[k++] = arr[i++];
    }
    while (j <= right) {
        temp[k++] = arr[j++];
    }
    for (i = left; i <= right; i++) {
        arr[i] = temp[i];
    }
}


/*
Helper for msort
*/
int my_msort_helper(int64_t* arr, int64_t* temp, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        #pragma omp task firstprivate(arr, temp, left, mid)
        {
            parallel_mergesort(arr, temp, left, mid);
        }

        #pragma omp task firstprivate(arr, temp, mid, right)
        {
            parallel_mergesort(arr, temp, mid + 1, right);
        }

        // Wait for the tasks to finish before merging
        #pragma omp taskwait
        merge(arr, temp, left, mid, right);
    }
    return 0;
}

/*
Sorts the input array and puts output back into the input array
*/

int my_sort(int64_t* arr, int size) {
    int64_t* temp = malloc(sizeof(int64_t) * size);
    if (temp == NULL) {
        fprintf(stderr, "Failed to allocate temporary array for mergesort\n");
        exit(EXIT_FAILURE);
    }

    #pragma omp parallel
    {
        #pragma omp single nowait
        {
            parallel_mergesort(arr, temp, 0, size - 1);
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
    uint64_t n; //The input size
    int64_t* input = Populate("./numbers.txt", &n); //gets the array
    /*
    printf("Array elements:\n");
    for (uint64_t i = 0; i < n; i++) {
        printf("%" PRId64 "\n", input[i]);
    }
    printf("\n\n\n");
    */
    my_msort(input, n);
    /*
    printf("Array elements:\n");
    for (uint64_t i = 0; i < n; i++) {
        printf("%" PRId64 "\n", input[i]);
    }
    printf("\n\n\n");
    */
    //check if it's sorted.
    int sorted = is_sorted(input, n);
    printf("Are the numbers sorted? %s \n", sorted ? "true" : "false");
   
    printf("Time elapsed: %lf \n", 0.0);
    free(input);
}