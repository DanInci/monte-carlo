#include <stdio.h>
#include "omp.h"
#include <stdlib.h>
#include<time.h>
#include <math.h>

//compute the sum of two arrays in parallel
#define N_def 10000

int state;

int main(int argc, char **argv) {
    unsigned long i, total_in = 0;
    unsigned long N = N_def;
  
    if(argc > 1) {
        N = atol(argv[1]);
    }
    srand(time(0));
    
    /* Compute values of array c = a+b in parallel. */
    #pragma omp threadprivate(state)
    #pragma omp parallel private(i) reduction(+:total_in)
    {
        #pragma omp single
        printf("Evaluating pi using %lu points on %d threads on %d procs\n", N, omp_get_num_threads(), omp_get_num_procs());
        unsigned long th_id = omp_get_thread_num();
        unsigned long th_in = 0;
        double x,y;
        #pragma omp for
        for (i = 0; i < N; i++) {
        x = 1.0*rand_r(&state)/RAND_MAX;
        y = 1.0*rand_r(&state)/RAND_MAX;
    //      printf("Thread %d: [%d]: (%.3f, %.3f) = %.3f\n", th_id, i, x, y, x*x + y * y);
    //      if(x*x + y*y < 1){
    //      th_in++;
    //      }
        th_in += ((x*x + y*y ) < 1);
        }
    //    #pragma omp critical
        {
    //    printf("Thread %lu found %lu points inside the circle\n", th_id, th_in);
            total_in += th_in;
        }
    }
    
    printf("Total in: %lu/%lu\n", total_in, N);
    double pi = 4.0*total_in/N;
    printf("Pi = %.10f ~= %.10f\n", pi, M_PI);
    int accuracy = 0;
    long decimals = 10;
    
    while((unsigned long) (pi*decimals) == (unsigned long) (M_PI*decimals)){
        accuracy++;
        decimals *= 10;
    }
    
    printf("Pi is estimated with accuracy of %d decimals\n", accuracy);
}
