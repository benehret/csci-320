#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <inttypes.h>
#include <omp.h>

int64_t samples_per_thread;
int64_t threads;
int64_t* output;

void find_pi() {
    int64_t count_inside_circle = 0;
    #pragma omp parallel num_threads(threads)
    {
        unsigned int seed = time(NULL) + omp_get_thread_num(); // Using thread-specific seed

        #pragma omp parallel reduction(+:count_inside_circle)
        for (int64_t i = 0; i < samples_per_thread; i++) {
            double x = (double)rand_r(&seed) / RAND_MAX * 1.0; // Random x coordinate between 0 and 1
            double y = (double)rand_r(&seed) / RAND_MAX * 1.0; // Random y coordinate between 0 and 1
            
            if (x * x + y * y <= 1.0) { // Check if point is inside the quarter-circle
                count_inside_circle++;
            }
        }
        
        output[omp_get_thread_num()] = count_inside_circle;
    }
}



int main(int argc, char** argv) {
    struct timespec start, end; // structs used for timing purposes
    double time_diff;
    int64_t n;
    n = (argc > 1) ? atoi(argv[1]) : 1000000;
    threads = (argc > 2) ? atoi(argv[2]) : 10;
    
    samples_per_thread = n / threads;

    clock_gettime(CLOCK_MONOTONIC, &start); // Start the clock!

    output = malloc(threads * sizeof(int64_t));

    find_pi();

    double sums = 0;
    for (int i = 0; i < threads; i++) {
        sums += output[i];
    }
    double pi = sums * 4 / n;
    free(output);

    clock_gettime(CLOCK_MONOTONIC, &end); // Stops the clock!

    time_diff = (end.tv_sec - start.tv_sec); //Difference in seconds
    time_diff += (end.tv_nsec - start.tv_nsec) / 1e9; //Difference in nanoseconds

    printf("The time taken is %f \n", time_diff);
    printf("This example uses %" PRId64 " terms.\n", n);
    printf("Pi is %.20lf\n", pi);

    return 0;
}
