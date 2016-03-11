/* File:     primes1.c
 * Purpose:  Find all primes less than or equal to an input value.
 *           This version doesn't bother checking even ints.
 *
 * Input:    n:  integer >= 2 (from command line)
 * Output:   Sorted list of primes between 2 and n,
 *
 * Compile:  gcc -g -Wall -o primes1 primes1.c -lm
 * Usage:    ./primes1 <n>
 *              n:  max int to test for primality
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void Usage(char prog[]);
int Get_n(int argc, char* argv[]);
int Is_prime(int i);

int main(int argc, char* argv[]) {
   int n, i;

   n = Get_n(argc, argv);

   printf("The primes < %d are\n", n);
   printf("2\n");
   for (i = 3; i <= n; i += 2)
      if (Is_prime(i))
         printf("%d\n", i);

   return 0;
}  /* main */

/*-------------------------------------------------------------------
 * Function:  Usage
 * Purpose:   Print a brief message explaining how the program is run.
 *            The quit.
 * In arg:    prog:  name of the executable
 */
void Usage(char prog[]) {
   fprintf(stderr, "usage: %s <n>\n", prog);
   fprintf(stderr, "   n = max integer to test for primality\n");
   exit(0);
}  /* Usage */

/*-------------------------------------------------------------------
 * Function:    Get_n
 * Purpose:     Get the input value n
 * Input args:  argc:  number of command line args
 *              argv:  array of command line args
 */
int Get_n(int argc, char* argv[]) {
   int n;

   if (argc != 2) Usage(argv[0]);

   /* Convert string to int */
   n = strtol(argv[1], NULL, 10);

   /* Check for bogus input */
   if (n <= 1) Usage(argv[0]);

   return n;
}  /* Get_n */

/*-------------------------------------------------------------------
 * Function:   Is_prime
 * Purpose:    Determine whether the argument is prime
 * Input arg:  i
 * Return val: true (nonzero) if arg is prime, false (zero) otherwise
 */
int Is_prime(int i) {
   int j;

   for (j = 2; j <= sqrt(i); j++)
      if (i % j == 0)
         return 0;
   return 1;
}  /* Is_prime */