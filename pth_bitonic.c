/* File:     pth_bitonic.c
 *
 * Purpose:  Implement bitonic sort of a list of ints using Pthreads
 *
 * Compile:  gcc -g -Wall -o pth_bitonic pth_bitonic.c -lpthread
 * Run:      ./pth_bitonic <thread count> <n> [g] [o]
 *           n = number of ints in the list 
 *           If 'g' is included on the command line, the program
 *              will use a random number generator to generate
 *              the list to be sorted.
 *           If 'o' is included on the command line, the program
 *              will print the original list and the sorted list
 *
 * Input:    If 'g' is not on the command line, user should enter
 *           the elements of the list
 *           
 * Output:   If 'o' is included on the command line, the original
 *           list and the sorted list.  
 *           The elapsed time for the sort.
 *
 * Notes:
 * 1.  thread_count should be a power of 2
 * 2.  n = list_size should be evenly divisible by thread_count
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

/* Random values in the range 0 to RMAX-1 */
#define RMAX 1000000
//#define RMAX 100

int thread_count;
//pthread_barrier_t barrier;
int bar_count = 0;
pthread_mutex_t bar_mutex;
pthread_cond_t bar_cond;
int n;
int *list1, *list2;
int *l_a, *l_b;

void Usage(char* prog_name);
void Get_args(int argc, char *argv[], int* gen_list_p, int* output_list_p);
void Gen_list(int list[], int n);
void Read_list(char prompt[], int list[], int n);
void Print_list(char title[], int list[], int n);
void *Bitonic_sort(void* rank);
void Bitonic_sort_incr(int th_count, int dim, int my_first, int local_n,
      int my_rank);
void Bitonic_sort_decr(int th_count, int dim, int my_first, int local_n,
      int my_rank);
void Merge_split_lo(int my_rank, int my_first, int local_n,
      int partner);
void Merge_split_hi(int my_rank, int my_first, int local_n,
      int partner);
int  Compare(const void* x_p, const void* y_p);
void Barrier(void);

/*--------------------------------------------------------------------*/
int main(int argc, char* argv[]) {
   long       thread;
   pthread_t* thread_handles; 
   double     start, finish;
   int        gen_list, output_list;

   Get_args(argc, argv, &gen_list, &output_list);

   thread_handles = malloc (thread_count*sizeof(pthread_t));
// pthread_barrier_init(&barrier, NULL, thread_count);
   pthread_mutex_init(&bar_mutex, NULL);
   pthread_cond_init(&bar_cond, NULL);
   list1 = malloc(n*sizeof(int));
   list2 = malloc(n*sizeof(int));
   l_a = list1;
   l_b = list2;

   if (gen_list)
      Gen_list(list1, n);
   else
      Read_list("Enter the list", list1, n);
   if (output_list)
      Print_list("The input list is", list1, n);

   GET_TIME(start);
   for (thread = 0; thread < thread_count; thread++)
      pthread_create(&thread_handles[thread], NULL,
          Bitonic_sort, (void*) thread);

   for (thread = 0; thread < thread_count; thread++) 
      pthread_join(thread_handles[thread], NULL);
   GET_TIME(finish);
   printf("Elapsed time = %e seconds\n", finish - start);

   if (output_list)
      Print_list("The sorted list is", l_a, n);

   free(list1);
   free(list2);
// pthread_barrier_destroy(&barrier);
   pthread_mutex_destroy(&bar_mutex);
   pthread_cond_destroy(&bar_cond);
   free(thread_handles);
   return 0;
}  /* main */


/*--------------------------------------------------------------------
 * Function:    Usage
 * Purpose:     Print command line for function and terminate
 * In arg:      prog_name
 */
void Usage(char* prog_name) {

   fprintf(stderr, "usage: %s <thread count> <n> [g] [o]\n", prog_name);
   fprintf(stderr, "n = number of elements in list\n");
   fprintf(stderr, "n should be evenly divisible by thread count\n");
   fprintf(stderr, "'g':  program should generate the list\n");
   fprintf(stderr, "'o':  program should output original and sorted lists\n");
   exit(0);
}  /* Usage */

/*-------------------------------------------------------------------
 * Function:    Get_args
 * Purpose:     Get command line args
 * In args:     argc, argv
 * Out args:    gen_list_p, output_list_p
 * Out global:  n
 */
void Get_args(int argc, char *argv[], int* gen_list_p, int* output_list_p) {
   char c1;

   if (argc < 3 || argc > 5) Usage(argv[0]);
   thread_count = strtol(argv[1], NULL, 10);
   n = strtol(argv[2], NULL, 10);
   if (n % thread_count != 0) Usage(argv[0]);

   // if (argc == 3)
   *gen_list_p = *output_list_p = 0;

   if (argc == 4) {
      c1 = argv[3][0];
      if (c1 == 'g') 
         *gen_list_p = 1;
      else
         *output_list_p = 1;
   } else if (argc == 5) {
      *gen_list_p = 1;
      *output_list_p = 1;
   }
}  /* Get_args */


/*-------------------------------------------------------------------
 * Function:  Gen_list
 * Purpose:   Use a random number generator to generate a list of ints
 * In arg:    n
 * Out arg:   list
 * In global: RMAX
 */
void Gen_list(int list[], int n) {
   int i;

   srandom(1);
   for (i = 0; i < n; i++)
      list[i] = random() % RMAX;
}  /* Gen_list */


/*-------------------------------------------------------------------
 * Function:  Read_list
 * Purpose:   Get a list of ints from stdin
 * In arg:    n
 * Out arg:   list
 */
void Read_list(char prompt[], int list[], int n) {
   int i;

   printf("%s\n", prompt);
   for (i = 0; i < n; i++)
      scanf("%d", &list[i]);
}  /* Read_list */


/*-------------------------------------------------------------------
 * Function:  Print_list
 * Purpose:   Print a list of ints to stdout
 * In args:   list, n
 */
void Print_list(char title[], int list[], int n) {
   int i;

   printf("%s:\n", title);
   for (i = 0; i < n; i++)
      printf("%d ", list[i]);
   printf("\n");
}  /* Print_list */


/*-----------------------------------------------------------------
 * Function:     Compare
 * Purpose:      Compare two ints and determine their relative sizes
 * In args:      x_p, y_p
 * Ret val:      -1 if *x_p < *y_p
 *                0 if *x_p == *y_p
 *               +1 if *x_p > *y_p
 * Note:         For use by qsort library function
 */
int Compare(const void* x_p, const void* y_p) {
   int x = *((int*)x_p);
   int y = *((int*)y_p);

   if (x < y)
      return -1;
   else if (x == y)
      return 0;
   else /* x > y */
      return 1;
}  /* Compare */


/*-------------------------------------------------------------------
 * Function:        Bitonic_sort
 * Purpose:         Implement bitonic sort of a list of ints
 * In arg:          rank
 * In globals:      barrier, thread_count, n (list size), list1
 * Out global:      l_a
 * Scratch globals: list2, l_b
 * Return val:      Ignored
 */
void *Bitonic_sort(void* rank) {
   long tmp = (long) rank;
   int my_rank = (int) tmp; 
   int local_n = n/thread_count;
   int my_first = my_rank*local_n;
// int my_last = my_first + local_n - 1;
   unsigned th_count, and_bit, dim;

   /* Sort my sublist */
   qsort(list1 + my_first, local_n, sizeof(int), Compare);  
   Barrier();
#  ifdef DEBUG
   if (my_rank == 0) Print_list("List after qsort", list1, n);
#  endif
   for (th_count = 2, and_bit = 2, dim = 1; th_count <= thread_count; 
         th_count <<= 1, and_bit <<= 1, dim++) {
      if ((my_rank & and_bit) == 0)
         Bitonic_sort_incr(th_count, dim, my_first, local_n, my_rank);
      else
         Bitonic_sort_decr(th_count, dim, my_first, local_n, my_rank);
   }

   return NULL;
}  /* Bitonic_sort */

/*-------------------------------------------------------------------
 * Function:      Bitonic_sort_incr
 * Purpose:       Use parallel bitonic sort to sort a list into
 *                   increasing order.  This implements a butterfly
 *                   communication scheme among the threads
 * In args:       th_count:  the number of threads participating
 *                   in this sort
 *                dim:  base 2 log of th_count
 *                my_first:  the subscript of my first element in l_a
 *                local_n:  the number of elements assigned to each
 *                   thread
 *                 my_rank:  the calling thread's global rank
 * In/out global:  l_a pointer to current list.
 * Scratch global: l_b pointer to temporary list.
 */
void Bitonic_sort_incr(int th_count, int dim, int my_first, int local_n,
      int my_rank) {
   int stage;
   int partner;
   int* tmp;
   unsigned eor_bit = 1 << (dim - 1);

   for (stage = 0; stage < dim; stage++) {
      partner = my_rank ^ eor_bit;
      if (my_rank < partner)
         Merge_split_lo(my_rank, my_first, local_n, partner);
      else
         Merge_split_hi(my_rank, my_first, local_n, partner);
      eor_bit >>= 1;
      Barrier();
      if (my_rank == 0) {
#        ifdef DEBUG
         char title[1000];
#        endif
         tmp = l_a;
         l_a = l_b;
         l_b = tmp;
#        ifdef DEBUG
         sprintf(title, "Th_count = %d, stage = %d", th_count, stage);
         Print_list(title, l_a, n);
#        endif
      }
      Barrier();
   } 
       
}  /* Bitonic_sort_incr */


/*-------------------------------------------------------------------
 * Function:      Bitonic_sort_decr
 * Purpose:       Use parallel bitonic sort to sort a list into
 *                   decreasing order.  This implements a butterfly
 *                   communication scheme among the threads
 * In args:       th_count:  the number of threads participating
 *                   in this sort
 *                dim:  base 2 log of th_count
 *                my_first:  the subscript of my first element in l_a
 *                local_n:  the number of elements assigned to each
 *                   thread
 *                 my_rank:  the calling thread's global rank
 * In/out global:  l_a pointer to current list.
 * Scratch global: l_b pointer to temporary list.
 */
void Bitonic_sort_decr(int th_count, int dim, int my_first, int local_n,
      int my_rank) {
   int stage;
   int partner;
   int* tmp;
   unsigned eor_bit = 1 << (dim - 1);

   for (stage = 0; stage < dim; stage++) {
      partner = my_rank ^ eor_bit;
      if (my_rank > partner)
         Merge_split_lo(my_rank, my_first, local_n, partner);
      else
         Merge_split_hi(my_rank, my_first, local_n, partner);
      eor_bit >>= 1;
      Barrier();
      if (my_rank == 0) {
#        ifdef DEBUG
         char title[1000];
#        endif
         tmp = l_a;
         l_a = l_b;
         l_b = tmp;
#        ifdef DEBUG
         sprintf(title, "Th_count = %d, stage = %d", th_count, stage);
         Print_list(title, l_a, n);
#        endif
      }
      Barrier();
   } 
       
}  /* Bitonic_sort_decr */


/*-------------------------------------------------------------------
 * Function:        Merge_split_lo
 * Purpose:         Merge two sublists in array l_a keeping lower half
 *                  in l_b
 * In args:         partner, local_n 
 * In/out global:   l_a
 * Scratch:         l_b
 */
void Merge_split_lo(int my_rank, int my_first, int local_n,
      int partner) {
   int ai, bi, xi, i;


   ai = bi = my_first;
   xi = partner*local_n;

#  ifdef DDEBUG
   printf("Th %d > In M_s_lo partner = %d, ai = %d, xi = %d\n",
         my_rank, partner, ai, xi);
#  endif    
   for (i = 0; i < local_n; i++)
      if (l_a[ai] <= l_a[xi]) {
         l_b[bi++] = l_a[ai++];
      } else {
         l_b[bi++] = l_a[xi++];
      }

}  /* Merge_split_lo */


/*-------------------------------------------------------------------
 * Function:        Merge_split_hi
 * Purpose:         Merge two sublists in array l_a keeping upper half
 *                  in l_b
 * In args:         partner, local_n 
 * In/out global:   l_a
 * Scratch:         l_b
 */
void Merge_split_hi(int my_rank, int my_first, int local_n,
      int partner) {
   int ai, bi, xi, i;

   ai = bi = my_first + local_n - 1;
   xi = (partner+1)*local_n - 1;

#  ifdef DDEBUG
   printf("Th %d > In M_s_hi partner = %d, ai = %d, xi = %d\n",
         my_rank, partner, ai, xi);
#  endif    

   for (i = 0; i < local_n; i++)
      if (l_a[ai] >= l_a[xi])
         l_b[bi--] = l_a[ai--];
      else
         l_b[bi--] = l_a[xi--];

}  /* Merge_split_hi */


/*-------------------------------------------------------------------
 * Function:  Barrier
 * Purpose:   Block all threads until all threads have called
 *            Barrier
 * Globals:   bar_count, bar_mutex, bar_cond
 */
void Barrier(void) {
   pthread_mutex_lock(&bar_mutex);
   bar_count++;
   if (bar_count == thread_count) {
      bar_count = 0;
      pthread_cond_broadcast(&bar_cond);
   } else {
      while (pthread_cond_wait(&bar_cond, &bar_mutex) != 0);
   }
   pthread_mutex_unlock(&bar_mutex);
}  /* Barrier */