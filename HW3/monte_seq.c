/* 
Bennett Ehret
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <inttypes.h>

double find_pi(int64_t n){
    int count_inside_circle = 0;
    srand(time(NULL));

    for (int i = 0; i < n; i++) {
        double x = (double)rand() / RAND_MAX * 1.0; // Random x coordinate between 0 and 1
        double y = (double)rand() / RAND_MAX * 1.0; // Random y coordinate between 0 and 1
        
        if (x * x + y * y <= 1.0) { // Check if point is inside the quarter-circle
            count_inside_circle++;
        }
    }

    return (double)count_inside_circle / n * 4; // Multiply by 4 to get pi
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