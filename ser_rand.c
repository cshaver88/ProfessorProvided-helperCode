/* File:  
 *    ser_rand.c
 *
 * Purpose:
 *    Generate random numbers with function that isn't threadsafe.
 *
 * Compile:
 *    gcc -g -Wall -o ser_rand ser_rand.c -lpthread
 * Usage:
 *    ser_rand <thread_count> <number of random numbers per thread>
 *
 * Input:
 *    None
 * Output:
 *    Random numbers from each thread
 *
 * Warning:
 *    The My_random function is *not* threadsafe.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define MR_MULTIPLIER 279470273 
#define MR_MODULUS 4294967291U


void Usage(char* prog_name);
unsigned My_random(unsigned seed);

/*--------------------------------------------------------------------*/
int main(int argc, char* argv[]) {
   long thread;
   int thread_count;
   int n, i;

   if (argc != 3) Usage(argv[0]);
   thread_count = strtol(argv[1], NULL, 10);
   n = strtol(argv[2], NULL, 10);

   for (thread = 0; thread < thread_count; thread++) {
      My_random(thread + 1);  /* "Seed" Random number generator */
      for (i = 0; i < n; i++) 
         printf("Th %ld > %u\n", thread, My_random(0));
   }

   return 0;
}  /* main */


/*--------------------------------------------------------------------
 * Function:    Usage
 * Purpose:     Print command line for function and terminate
 * In arg:      prog_name
 */
void Usage(char* prog_name) {

   fprintf(stderr, "usage: %s <thread count> <number of random vals per thread>\n", 
         prog_name);
   exit(0);
}  /* Usage */


/*-------------------------------------------------------------------
 * Function:    My_random
 * Purpose:     Generate random numbers
 * In arg:      seed  (ignored if = 0)
 * Ret val:     A "random" unsigned int
 *
 * Note:        The first time the function is called, the random
 *              number should be "seeded" with a nonzero argument,
 *              and the return value ignored.  Subsequent calls 
 *              should have a zero argument and the return values
 *              will form a "pseudo-random" sequence.
 */
unsigned My_random(unsigned seed) {
   static unsigned z;
   unsigned long tmp;

   if (seed != 0) z = seed;
   
   tmp = z*MR_MULTIPLIER; 
   z = tmp % MR_MODULUS;
   return z;
}  /* My_random */