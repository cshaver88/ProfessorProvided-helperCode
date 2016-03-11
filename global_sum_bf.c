/* File:     global_sum_bf.c
 *
 * Purpose:  Implements a butterfly-structured global sum.
 * 
 * Input:    None.
 * Output:   Random values generated by processes, and sum of random values.
 *
 * Compile:  mpicc -g -Wall -o global_sum global_sum.c
 * Run:      mpiexec -n <number of processes> global_sum
 *
 * Notes:     
 *    1.  p, the number of processes, *must* be a power of 2.
 *    2.  The result returned by all the processes is valid.
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
 * Notes:
 *    1.  Uses butterfly structured communication.
 *    2.  p, the number of processes must be a power of 2.
 *    3.  The pairing of the processes is done using bitwise
 *        exclusive or.  Here's a table showing the rule for
 *        for bitwise exclusive or
 *           X Y X^Y
 *           0 0  0
 *           0 1  1
 *           1 0  1
 *           1 1  0
 *        Here's a table showing the process pairing with 8
 *        processes (r = my_rank, other column heads are bitmask)
 *           r     001 010 100 
 *           -     --- --- ---
 *           0 000 001 010 110  
 *           1 001 000 011 101
 *           2 010 011 000 110
 *           3 011 010 001 111
 *           4 100 101 110 000
 *           5 101 100 111 001
 *           6 110 111 100 010
 *           7 111 110 101 011
 */
int Global_sum(int my_contrib, int my_rank, int p, MPI_Comm comm) {
    int        sum = my_contrib;
    int        temp;
    int        partner;
    unsigned   bitmask = 1;

    while (bitmask < p) {
        partner = my_rank ^ bitmask;
        MPI_Sendrecv(&sum, 1, MPI_INT, partner, 0, 
                    &temp, 1, MPI_INT, partner, 0, 
                    comm, MPI_STATUS_IGNORE);
        sum += temp;
        bitmask <<= 1;
    }

    return sum;
}  /* Global_sum */
