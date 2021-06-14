#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define N_def 10000

int main(int argc, char **argv){
   int in_circle = 0, i, N;
   float x, y;
   
   if(argc > 1) {
      N = atoi(argv[1]);
   } else {
      N = N_def;
   }
   
   printf("Evaluating PI with %d points", N);
   srand(time(0));
   for(i = 0; i < N; i++){
      x = 1.0*rand()/RAND_MAX;
      y = 1.0*rand()/RAND_MAX;
//      printf("(%.3f, %.3f", x, y);
     if(x*x + y*y < 1){
        in_circle++;
     }
   
   }

   printf("INside: %d/%d\n", in_circle, N);
   float pi = 4.0*in_circle/N;
   printf("Pi = %.10f ~= %.10f\n", pi, M_PI);
   int accuracy = 0;
   long decimals = 10;
      
   while((unsigned long) (pi*decimals) == (unsigned long) (M_PI*decimals)){
      accuracy++;
      decimals *= 10;
   }

   printf("Pi is estimated with accuracy of %d decimals\n", accuracy);
}
