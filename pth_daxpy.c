/* File:     
 *     pth_daxpy.c 
 *
 * Purpose:  
 *     Computes a DAXPY, which is an update y += alpha*x, where
 *     x and y are vectors, and alpha is a scalar.  This program
 *     uses a block distribution of the vectors.
 *
 * Input:
 *     n: dimension of vectors
 *     x, y: the vectors
 *     alpha:  the scalar
 *
 * Output:
 *     y: the vector y after the update
 *
 * Compile:  gcc -g -Wall -o pth_daxpy pth_daxpy.c -lpthread
 * Usage:
 *     pth_daxpy <thread_count>
 *
 * Notes:  
 *     1.  Local storage for x and y is dynamically allocated.
 *     2.  Distribution of x, and y is logical:  both are 
 *         globally shared.
 *     3.  n should be evenly divisible by thread_count.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* Global variables */
int     thread_count;
int     n;
double* x;
double* y;
double  alpha;

/* Serial functions */
void Usage(char* prog_name);
void Read_vector(char* prompt, double x[], int n);
void Print_vector(char* title, double y[], int m);

/* Parallel function */
void *Pth_daxpy(void* rank);

/*------------------------------------------------------------------*/
int main(int argc, char* argv[]) {
   long       thread;
   pthread_t* thread_handles;

   if (argc != 2) Usage(argv[0]);
   thread_count = strtol(argv[1],NULL,10);
   thread_handles = malloc(thread_count*sizeof(pthread_t));

   printf("Enter n\n");
   scanf("%d", &n);

   x = malloc(n*sizeof(double));
   y = malloc(n*sizeof(double));
   
   Read_vector("Enter the vector x", x, n);
   Print_vector("We read", x, n);

   Read_vector("Enter the vector y", y, n);
   Print_vector("We read", y, n);

   printf("Enter alpha\n");
   scanf("%lf", &alpha);
   printf("We read %lf\n", alpha);

   for (thread = 0; thread < thread_count; thread++)
      pthread_create(&thread_handles[thread], NULL,
         Pth_daxpy, (void*) thread);

   for (thread = 0; thread < thread_count; thread++)
      pthread_join(thread_handles[thread], NULL);

   Print_vector("The product is", y, n);

   free(x);
   free(y);
   free(thread_handles);

   return 0;
}  /* main */


/*------------------------------------------------------------------
 * Function:  Usage
 * Purpose:   print a message showing what the command line should
 *            be, and terminate
 * In arg :   prog_name
 */
void Usage (char* prog_name) {
   fprintf(stderr, "usage: %s <thread_count>\n", prog_name);
   exit(0);
}  /* Usage */

/*------------------------------------------------------------------
 * Function:        Read_vector
 * Purpose:         Read in the vector x
 * In arg:          prompt, n
 * Out arg:         x
 */
void Read_vector(char* prompt, double x[], int n) {
   int   i;

   printf("%s\n", prompt);
   for (i = 0; i < n; i++) 
      scanf("%lf", &x[i]);
}  /* Read_vector */


/*------------------------------------------------------------------
 * Function:          Pth_daxpy
 * Purpose:           Update a double precision vector y += alpha*x
 * In arg:            rank
 * Global in vars:    x, n, alpha, thread_count
 * Global in/out var: y
 */
void *Pth_daxpy(void* rank) {
   long my_rank = (long) rank;
   int local_n = n/thread_count;
   int my_first_i = my_rank*local_n;
   int my_last_i = my_first_i + local_n;
   int i;

   for (i = my_first_i; i < my_last_i; i ++) {
      y[i] += alpha*x[i];
   }

   return NULL;
}  /* Pth_mat_vect */


/*------------------------------------------------------------------
 * Function:    Print_vector
 * Purpose:     Print a vector
 * In args:     title, y, m
 */
void Print_vector(char* title, double y[], int m) {
   int   i;

   printf("%s\n", title);
   for (i = 0; i < m; i++)
      printf("%4.1f ", y[i]);
   printf("\n");
}  /* Print_vector */