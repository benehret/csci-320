#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <time.h>
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

    int64_t* array = (int64_t*)malloc(*size * sizeof(int64_t));
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
TODO: Computes the prefix sum of in, with size number of elements. Store the result into out.
Maybe you want to return something useful like the total sum?
*/
int64_t psum_seq(int64_t* in, int64_t* out, uint64_t size){
    for (uint64_t i = 0; i < size; i++){
        if (i!=0){
            *(out+i) = *(in+i) + *(out+i-1);
        } else {
            *(out+i) = *(in+i);
        }
    }
    return *(out+size-1);
}


int64_t psum_par(int64_t* in, int64_t* out, uint64_t size, int64_t* temp){
    #pragma omp parallel for
    for (int64_t i = 0; i < size; i++){
        out[i] = in[i];
    }
    for (int64_t pwrs = 1; pwrs <= size; pwrs*=2) {
        #pragma omp parallel for
        for (int64_t i = 0; i < size; i++){
            temp[i] = out[i];
        }
        #pragma omp parallel for
        for (int64_t i = pwrs; i < size; i++){
            out[i] += temp[i-pwrs];
        }
    }
    return out[size-1];
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
TODO: Partitions the array and returns the index of the pivot (after partition) in parallel
*/
uint64_t partition_par(int64_t* input, uint64_t size, int64_t* aux, int64_t* pos, int64_t* out) {
    int64_t pivot_ind = size - 1;
    #pragma omp parallel for
    for (int64_t i = 0; i < size; i++){
        if (input[i] <= input[pivot_ind]){
            aux[i] = 1;
        } else {
            aux[i] = 0;
        }
    }


    psum_par(aux, pos, size, out);


    #pragma omp parallel for
    for (int64_t i = 0; i < size; i++){
        out[i] = input[i];
    }

    #pragma omp parallel for
    for(int64_t i = 0; i < size; i++){
        if (i == 0 || pos[i] != pos[i-1]){
            out[pos[i]-1] = input[i];
        } 
    }
    uint64_t newin = pos[pivot_ind]-1;
    #pragma omp parallel for
    for (int64_t i = 0; i < size; i++){
        if (input[i] > input[pivot_ind]){
            aux[i] = 1;
        } else {
            aux[i] = 0;
        }
    }
    
    aux[0] += newin + 1;

    psum_par(aux, pos, size, out);
    #pragma omp parallel for
    for(int64_t i = 0; i < size; i++){
        if (i == 0 || pos[i] != pos[i-1]){
            out[pos[i]-1] = input[i];
        } 
    }
    out[newin] = input[pivot_ind];
    if (!input || !out) {
        fprintf(stderr, "Error: One of the arrays has not been allocated.\n");
        exit(EXIT_FAILURE);
    }
    #pragma omp parallel for
    for (int64_t i = 0; i < size; i++){
        input[i] = out[i];
    }

    return newin;
    
}

int my_qsort_helper(int64_t* input, uint64_t size, int64_t* aux, int64_t* pos, int64_t* out){
    if (size > 1000000000000) {
        uint64_t p = partition_par(input, size, aux, pos, out);
        #pragma omp task firstprivate(p)
        my_qsort_helper(input, p, aux, pos, out);
        #pragma omp task firstprivate(p)
        my_qsort_helper(&input[p], size - p, &aux[p], &pos[p], &out[p]);
    } else if (size > 1){
        uint64_t p = partition(input, size);
        #pragma omp task firstprivate(p)
        my_qsort_helper(input ,p , aux, pos, out);
        #pragma omp task firstprivate(p)
        my_qsort_helper(&input[p] + 1, size - p - 1, &aux[p], &pos[p], &out[p]);
    }
    #pragma omp taskwait
    return 0;
}

/*
Sorts the input array and puts output back into the input array
*/
int my_qsort(int64_t* input, uint64_t size){
    int64_t* aux = (int64_t*)malloc(size * sizeof(int64_t));
    int64_t* pos = (int64_t*)malloc(size * sizeof(int64_t));
    int64_t* out = (int64_t*)malloc(size * sizeof(int64_t));
    
    #pragma omp parallel
    {
        #pragma omp single
        {
            my_qsort_helper(input, size, aux, pos, out);
        }
    }

    #pragma omp taskwait
    if (aux) { free(aux); aux = NULL; }
    if (pos) { free(pos); pos = NULL; }
    if (out) { free(out); out = NULL; }
    return 0;
}

/*
Returns 0 if not sorted, returns 1 if sorted
TODO: Modify if necessary
*/
int is_sorted(int64_t* input, uint64_t size){
	for (uint64_t i = 1; i < size; i++){
		if (input[i-1] > input[i]){
            printf("MISMATSCH: i-1 = %" PRId64 " i = %"PRId64" at index: %" PRId64 " and %" PRId64 "\n\n", input[i-1] , input[i], i-1, i);
			//return 0;
		}	
	}
    return 1;
}

/*
Returns 0 if the prefix sums agree, returns 1 if they agree
*/
int test_psum(int64_t* input, uint64_t size){
	int64_t* out_seq = malloc(size * sizeof(int64_t));
	int64_t* out_par = malloc(size * sizeof(int64_t));
    int64_t* temp = malloc(size * sizeof(int64_t));
    
    int64_t tot_s = psum_seq(input, out_seq, size);
    int64_t tot_p = psum_par(input, out_par, size, temp);
    printf("Prefix Sum Sequential Total: %"PRId64"\n", tot_s);
    printf("Prefix Sum Parallel Total: %"PRId64"\n", tot_p);
    for (uint64_t i = 0; i < size; i ++){
        if (out_seq[i]!=out_par[i]){
            printf("Something didn't match?!\n");
            return 0;   //Something doesn't match! Leaks memory a bit but okay.
        }
    }
    free(out_par);
    free(out_seq);
    return 1;
}

int main(int argc, char** argv){
    struct timespec start, end; //structs used for timing purposes, it has two memebers, a tv_sec which is the current second, and the tv_nsec which is the current nanosecond.
    double time_diff;
    uint64_t n; //The input size
    int64_t* input = Populate("./numbers.txt", &n); //gets the array


    /*
    printf("Array elements:\n");
    for (uint64_t i = 0; i < n; i++) {
        printf("%" PRId64 "\n", input[i]);
    }
    printf("\n\n\n");
    */
    /*
    for (int i = 0; i < n; i++){
        printf("IN[%d] = %" PRId64"\n", i, input[i]);
    }
    */
    clock_gettime(CLOCK_MONOTONIC, &start); //Start the clock!
    //test_psum(test,8);
    //free(test);
    my_qsort(input, n);
    clock_gettime(CLOCK_MONOTONIC, &end);   //Stops the clock!
    /*
    printf("Array elements:\n");
    for (uint64_t i = 0; i < n; i++) {
        printf("%" PRId64 "\n", input[i]);
    }
    printf("\n\n\n");
    */
    //check if it's sorted.
    //int sorted = is_sorted(input, n);
    //printf("Are the numbers sorted? %s \n", sorted ? "true" : "false");
    /*
    for (int i = 0; i < n; i++){
        printf("OUT[%d] = %" PRId64"\n", i, input[i]);
    }
    */
    time_diff = (end.tv_sec - start.tv_sec); //Difference in seconds
    time_diff += (end.tv_nsec - start.tv_nsec) / 1e9; //Difference in nanoseconds

    printf("Time elapsed: %lf seconds\n", time_diff);
    free(input);
}