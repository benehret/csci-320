/* 
Bennett Ehret
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <inttypes.h>
#include <pthread.h>

int64_t samples_per_thread;
intptr_t threads;
int64_t* output;

void* find_pi(void* t_number){
    intptr_t num = (intptr_t)t_number;

    int count_inside_circle = 0;
    srand(time(NULL)+num);

    for (int i = 0; i < samples_per_thread; i++) {
        double x = (double)rand() / RAND_MAX * 1.0; // Random x coordinate between 0 and 1
        double y = (double)rand() / RAND_MAX * 1.0; // Random y coordinate between 0 and 1
        
        if (x * x + y * y <= 1.0) { // Check if point is inside the quarter-circle
            count_inside_circle++;
        }
    }
    output[num] = count_inside_circle;
    return 0;
}

int main(int argc, char** argv){
    struct timespec start, end; //structs used for timing purposes, it has two memebers, a tv_sec which is the current second, and the tv_nsec which is the current nanosecond.
    double time_diff;
    int64_t n;
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
    
    samples_per_thread = n / threads;

    clock_gettime(CLOCK_MONOTONIC, &start); //Start the clock!
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
    double pi = sums * 4 / n;
    free(output);
    free(handlers);
    clock_gettime(CLOCK_MONOTONIC, &end);   //Stops the clock!

    time_diff = (end.tv_sec - start.tv_sec); //Difference in seconds
    time_diff += (end.tv_nsec - start.tv_nsec) / 1e9; //Difference in nanoseconds

    printf("The time taken is %f \n", time_diff);
    printf("This example uses %" PRId64 " terms.\n", n);
    printf("Pi is %.20lf\n", pi);
}