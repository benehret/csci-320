/* 
Bennett Ehret
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <math.h>
#include <inttypes.h>

double find_pi(int64_t n){
    double Pi;
    Pi = 0;
    for (int64_t i = 0; i < n; i++){
        Pi += pow(-1, i) * (1.0/(2*i+1));
    }
    Pi *= 4.0;
    return Pi; 
}

int main(int argc, char** argv){
    struct timespec start, end; //structs used for timing purposes, it has two memebers, a tv_sec which is the current second, and the tv_nsec which is the current nanosecond.
    double time_diff;
    int64_t n;
    if (argc == 1){
        n = 1000000;
    } else {
        n = (int64_t) atoi(argv[1]);
    }

    clock_gettime(CLOCK_MONOTONIC, &start); //Start the clock!
    double pi = find_pi(n);
    clock_gettime(CLOCK_MONOTONIC, &end);   //Stops the clock!

    time_diff = (end.tv_sec - start.tv_sec); //Difference in seconds
    time_diff += (end.tv_nsec - start.tv_nsec) / 1e9; //Difference in nanoseconds

    printf("The time taken is %f \n", time_diff);
    printf("This example uses %" PRId64 " terms.\n", n);
    printf("Pi is %.20lf\n", pi);
}