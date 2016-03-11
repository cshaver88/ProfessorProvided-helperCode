/* File:    serial_odd_even_timed.c
 *
 * Purpose: Time odd-even transposition sort
 *
 * Compile: gcc -g -Wall -O3 -o soe serial_odd_even_timed.c
 * Usage:   odd_even <n> <g|i>
 *             n:   number of elements in list
 *            'g':  generate list using a random number generator
 *            'i':  user input list (not used)
 *
 * Input:   none
 * Output:  elapsed wall clock time for sort
 *
 * Note:  The program does no I/O other than printing the elapsed time.
 *
 * Timing data is in the file serial_bubble_timed.c
 */
#include <stdio.h>
#include <stdlib.h>
#include "timer.h"

const int RMAX = 1000000000;

void Usage(char* prog_name);
void Get_args(int argc, char* argv[], int* n_p, char* g_i_p);
void Generate_list(int a[], int n);
void Print_list(int a[], int n, char* title);
void Read_list(int a[], int n);
void Odd_even_sort(int a[], int n);
void Odd_even_iter(int a[], int n, int phase);
void Swap(int* x_p, int* y_p);

/*-----------------------------------------------------------------*/
int main(int argc, char* argv[]) {
   int  n;
   char g_i;
   int* a;
   double start, finish;

   Get_args(argc, argv, &n, &g_i);
   a = (int*) malloc(n*sizeof(int));
   if (g_i == 'g') {
      Generate_list(a, n);
//    Print_list(a, n, "Before sort");
   } else {
      Read_list(a, n);
   }

   GET_TIME(start);
   Odd_even_sort(a, n);
   GET_TIME(finish);
   printf("Elapsed time to sort %d ints using odd-even sort = %e seconds\n", 
         n, finish-start);

// Print_list(a, n, "After sort");
   
   free(a);
   return 0;
}  /* main */


/*-----------------------------------------------------------------
 * Function:  Usage
 * Purpose:   Summary of how to run program
 */
void Usage(char* prog_name) {
   fprintf(stderr, "usage:   %s <n> <g|i>\n", prog_name);
   fprintf(stderr, "   n:   number of elements in list\n");
   fprintf(stderr, "  'g':  generate list using a random number generator\n");
   fprintf(stderr, "  'i':  user input list\n");
}  /* Usage */


/*-----------------------------------------------------------------
 * Function:  Get_args
 * Purpose:   Get and check command line arguments
 * In args:   argc, argv
 * Out args:  n_p, g_i_p
 */
void Get_args(int argc, char* argv[], int* n_p, char* g_i_p) {
   if (argc != 3 ) {
      Usage(argv[0]);
      exit(0);
   }
   *n_p = atoi(argv[1]);
   *g_i_p = argv[2][0];

   if (*n_p <= 0 || (*g_i_p != 'g' && *g_i_p != 'i') ) {
      Usage(argv[0]);
      exit(0);
   }
}  /* Get_args */


/*-----------------------------------------------------------------
 * Function:  Generate_list
 * Purpose:   Use random number generator to generate list elements
 * In args:   n
 * Out args:  a
 */
void Generate_list(int a[], int n) {
   int i;

   srandom(0);
   for (i = 0; i < n; i++)
      a[i] = random() % RMAX;
}  /* Generate_list */


/*-----------------------------------------------------------------
 * Function:  Print_list
 * Purpose:   Print the elements in the list
 * In args:   a, n
 */
void Print_list(int a[], int n, char* title) {
   int i;

   printf("%s:\n", title);
   for (i = 0; i < n; i++)
      printf("%d ", a[i]);
   printf("\n\n");
}  /* Print_list */


/*-----------------------------------------------------------------
 * Function:  Read_list
 * Purpose:   Read elements of list from stdin
 * In args:   n
 * Out args:  a
 */
void Read_list(int a[], int n) {
   int i;

   printf("Please enter the elements of the list\n");
   for (i = 0; i < n; i++)
      scanf("%d", &a[i]);
}  /* Read_list */


/*-----------------------------------------------------------------
 * Function:     Odd_even_sort
 * Purpose:      Sort list using odd-even transposition sort
 * In args:      n
 * In/out args:  a
 */
void Odd_even_sort(int a[], int n) {
   int phase;
#  ifdef DEBUG
   char title[100];
#  endif

   for (phase = 0; phase < n; phase++) {
      Odd_even_iter(a, n, phase);
#     ifdef DEBUG
      sprintf(title, "After phase %d", phase);
      Print_list(a, n, title);
#     endif
   }
}  /* Odd_even_sort */

/*-----------------------------------------------------------------
 * Function:    Odd_even_iter
 * Purpose:     Execute one iteration of odd-even transposition sort
 * In args:     n, phase
 * In/out args: a
 */
void Odd_even_iter(int a[], int n, int phase) {
   int i, left, right;

   if (phase % 2 == 0) {  /* Even phase:  odd subscripts look left  */
      for (i = 1; i < n; i += 2) {
         left = i-1;
         if (a[left] > a[i]) Swap(&a[left],&a[i]);
      }
   } else {  /* Odd phase:  odd subscripts look right */
      for (i = 1; i < n-1; i += 2) {
         right = i+1;
         if (a[i] > a[right]) Swap(&a[i], &a[right]);
      }
   }
}  /* Odd_even_iter */
      
/*-----------------------------------------------------------------
 * Function:     Swap
 * Purpose:      Swap contents of x_p and y_p
 * In/out args:  x_p, y_p
 */
void Swap(int* x_p, int* y_p) {
   int temp = *x_p;
   *x_p = *y_p;
   *y_p = temp;
}  /* Swap */