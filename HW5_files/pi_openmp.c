#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <math.h>
#include <inttypes.h>
#include <omp.h>

int64_t n;
double* output;

void find_pi() {
    int num_threads = omp_get_num_threads();
    int num = omp_get_thread_num();
    // Calculating the "section" of the series that the respective thread will work on.
    double section_size = (double)n / num_threads;
    int extra = (section_size - (int)section_size) * (num + 1);
    int lower = section_size * num;
    int higher = lower + section_size;

    // Calculating sum of sequence for that section
    double Pi = 0;
    for (intptr_t i = lower; i < higher; i++) {
        Pi += pow(-1, i) * (1.0 / (2 * i + 1));
    }
    // If the number of terms being divided into threads does not go evenly,
    // This will calculate the additional term at the end of the last window that is missed.
    if (higher == (int)section_size * num_threads) {
        for (int i = 0; i < extra; i++) {
            int val = higher + i;
            Pi += pow(-1, val) * (1.0 / (2 * val + 1));
        }
    }

    output[num] = Pi;
}

int main(int argc, char** argv) {
    struct timespec start, end; // structs used for timing purposes
    double time_diff;
    n = (argc > 1) ? atoi(argv[1]) : 1000000;
    int num_threads = (argc > 2) ? atoi(argv[2]) : 10;

    clock_gettime(CLOCK_MONOTONIC, &start); // Start the clock!
    output = malloc(num_threads * sizeof(double));
    // OpenMP parallel region
    #pragma omp parallel num_threads(num_threads)
    {
	find_pi();
    }

    double sums = 0;
    for (int i = 0; i < num_threads; i++) {
        sums += output[i];
    }
    double pi = sums * 4;
    free(output);

    clock_gettime(CLOCK_MONOTONIC, &end); // Stops the clock!

    time_diff = (end.tv_sec - start.tv_sec); //Difference in seconds
    time_diff += (end.tv_nsec - start.tv_nsec) / 1e9; //Difference in nanoseconds

    printf("Pi is %.20lf\n", pi);
    printf("The number of terms used: %" PRId64 "\n", n);
    printf("The time taken is %f seconds\n", time_diff);

    return 0;
}
