/* File:     global_sum_driver.c
 *
 * Purpose:  Driver for a program that uses MPI to implement a global
 *           sum
 * 
 * Input:    None.
 * Output:   Random values generated by processes and sum of random 
 *           values on each process.
 *
 * Compile:  mpicc -g -Wall -o gs global_sum_driver.c
 * Run:      mpiexec -n <number of processes> gs
 *
 * Notes:     
 *    1.  The result returned by all the processes should be valid.
 *    2.  The Global_sum function is not implemented.
 */
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

const int MAX_CONTRIB = 20;

int Global_sum(int my_contrib, int my_rank, int p, MPI_Comm comm);
void Print_results(char title[], int value, int my_rank, int p,
      MPI_Comm comm);

int main(void) {
   int      p, my_rank;
   MPI_Comm comm;
   int      my_contrib;
   int      sum;

   MPI_Init(NULL, NULL);
   comm = MPI_COMM_WORLD;
   MPI_Comm_size(comm, &p);
   MPI_Comm_rank(comm, &my_rank);

   /* Generate a random int */
   srandom(my_rank);
   my_contrib = random() % MAX_CONTRIB;

   Print_results("Process Values", my_contrib, my_rank, p, comm);

   sum = Global_sum(my_contrib, my_rank, p, comm);

   Print_results("Process Totals", sum, my_rank, p, comm);

   MPI_Finalize();
   return 0;
} 
/*---------------------------------------------------------------
 * Function:  Print_results
 * Purpose:   Gather an int from each process onto process 0 and
 *            print the values.
 * In args:   
 *    title:  the title of the output
 *    value:  the int contributed by each process
 *    my_rank, p, comm:  the usual MPI values
 */
void Print_results(char title[], int value, int my_rank, int p,
      MPI_Comm comm) {
   int* vals = NULL, q;

   if (my_rank == 0) {
      vals = malloc(p*sizeof(int));
      MPI_Gather(&value, 1, MPI_INT, vals, 1, MPI_INT, 0, comm);
      printf("%s:\n", title);
      for (q = 0; q < p; q++)
         printf("Proc %d > %d\n", q, vals[q]);
      printf("\n");
      free(vals);
   } else {
      MPI_Gather(&value, 1, MPI_INT, vals, 1, MPI_INT, 0, comm);
   }
}  /* Print_results */


/*-----------------------------------------------------------------
 * Function:    Global_sum
 * Purpose:     Compute the global sum of ints stored on the processes
 *
 * Input args:  my_contrib = process's contribution to the global sum
 *              my_rank = process's rank
 *              p = number of processes
 *              comm = communicator
 * Return val:  Sum of each process's my_contrib:  valid on all
 *              processes
 *
 */
int Global_sum(int my_contrib, int my_rank, int p, MPI_Comm comm) {
   int sum = my_contrib; 
   return sum;
}  /* Global_sum */