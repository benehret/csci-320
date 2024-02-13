/* 
Bennett Ehret
*/
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <math.h>
#include <inttypes.h>
#include <pthread.h>

int64_t n;
intptr_t threads;
double* output;

void* find_pi(void* t_number){
    intptr_t num = (intptr_t)t_number;
    //Calculating the "section" of the series that the respective thread will work on.
    double section_size = (double) n/threads;
    int extra = (section_size - (int)section_size) * (num + 1);
    int lower = section_size * num;
    int higher = lower + section_size;

    //Calculating sum of sequence for that section
    double Pi;
    Pi = 0;
    for (intptr_t i = lower; i < higher; i++){
        Pi += pow(-1, i) * (1.0/(2*i+1));
    }
    // If the number of terms being divided into threads does not go evenly, 
    // This will calculate the additional term at the end of the last window that is missed.
    if(higher == (int) section_size * threads){
        for (int i = 0; i < extra; i++){
            int val = higher + i;
            Pi += pow(-1, val) * (1.0/(2*val+1));
        }
    }

    output[num] = Pi;
    return 0; 
}

int main(int argc, char** argv){
    struct timespec start, end; //structs used for timing purposes, it has two memebers, a tv_sec which is the current second, and the tv_nsec which is the current nanosecond.
    double time_diff;
    if (argc == 1){
        n = (int64_t) 1000000;
        threads = (intptr_t)10;
    } else if (argc == 2){
        n = (int64_t) atoi(argv[1]);
        threads = (intptr_t) 10;
    } else {
        n = (int64_t) atoi(argv[1]);
        threads = (intptr_t) atoi(argv[2]);
    }
    
    clock_gettime(CLOCK_MONOTONIC, &start); //Start the clock!
    // Creating threads
    pthread_t* handlers = malloc(threads*sizeof(pthread_t));
    output = malloc(threads*sizeof(double));
    for (intptr_t i = 0; i < threads; i++){
        pthread_create(&handlers[i],NULL,find_pi,(void*)i);
    }

    for(intptr_t i = 0; i < threads; i++){
        pthread_join(handlers[i],NULL);
    }

    double sums = 0;
    for (int i = 0; i < threads; i++){
        sums += output[i];
    }
    double pi  = sums * 4;
    free(output);
    free(handlers);
    clock_gettime(CLOCK_MONOTONIC, &end);   //Stops the clock!

    time_diff = (end.tv_sec - start.tv_sec); //Difference in seconds
    time_diff += (end.tv_nsec - start.tv_nsec) / 1e9; //Difference in nanoseconds

    printf("The time taken is %f \n", time_diff);
    printf("This example uses %" PRId64 " terms.\n", n);
    printf("Pi is %.20lf\n", pi);
}