/* File:      bcast.c
 * Purpose:   Driver for MPI tree structured broadcast function.
 * Input:     A single int
 * Output:    value received by each process.
 *
 * Algorithm: 
 *    1. Process 0 reads in an it
 *    2. Processes call Bcast
 *    3. Each process prints value it received
 *
 * Compile:  mpicc -g -Wall -o bcast bcast.c
 * Run:      mpiexec -n <number of processes> bcast
 *
 * Notes:
 *    1. This version has no restriction on the number of processes.
 *       (This wasn't required in the homework.)
 *    2. This uses the tree
 *       0->1
 *       0->2, 1->3
 *       0->4, 1->5, 2->6, 3->7
 *       etc.
 *    3. DEBUG flag can be used to print information on pairing
 *       of processes.
 */
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int Bcast(int in_val, int my_rank, int p, MPI_Comm comm);

int main(int argc, char* argv[]) {
   int p, my_rank;
   MPI_Comm comm;
   int result, in_val;

   MPI_Init(&argc, &argv);
   comm = MPI_COMM_WORLD;
   MPI_Comm_size(comm, &p);
   MPI_Comm_rank(comm, &my_rank);

   if (my_rank == 0) {
      printf("Enter an int\n");
      scanf("%d", &in_val);
   }

   result = Bcast(in_val, my_rank, p, comm);

   printf("Proc %d > result = %d\n", my_rank, result); 

   MPI_Finalize();
   return 0;
}  /* main */


/*-----------------------------------------------------------------*/
/* Function:    Bcast
 * Purpose:     Broadcast a value from process 0 to all the other
 *              processes
 *
 * Input args:  in_val = the value to be broadcast from process 0
 *              my_rank = process's rank
 *              p = number of processes
 *              comm = communicator
 * Return val:  on each process the value broadcast by process 0
 *
 * Notes:
 *    1.  Uses tree structured communication.
 *    2.  The pairing of the processes is done using bitwise
 *        exclusive or.  Here's a table showing the rule for
 *        for bitwise exclusive or
 *           X Y X^Y
 *           0 0  0
 *           0 1  1
 *           1 0  1
 *           1 1  0
 *        Here's a table showing the process pairing with 8
 *        processes (r = my_rank, other column heads are 
 *        bitmask)
 *           r     001 010 100 
 *           -     --- --- ---
 *           0 000 001 010 100  
 *           1 001 000 011 101
 *           2 010  x  000 110
 *           3 011  x  001 111
 *           4 100  x   x  000
 *           5 101  x   x  001
 *           6 110  x   x  010
 *           7 111  x   x  011
 */
int Bcast(int in_val, int my_rank, int p, MPI_Comm comm) {
   int        partner;
   unsigned   bitmask = 1;
   int        participate = bitmask << 1;

#  ifdef DEBUG
   printf("Proc %d > bitmask = %d\n", my_rank, bitmask);
   fflush(stdout);
#  endif
   while (bitmask < p) {
      if (my_rank < participate) {
         partner = my_rank ^ bitmask;
#        ifdef DEBUG
         printf("Proc %d > bitmask = %d, partner = %d\n",
            my_rank, bitmask, partner);
         fflush(stdout);
#        endif
         if (my_rank < partner) {
            if (partner < p )
               MPI_Send(&in_val, 1, MPI_INT, partner, 0, comm);
         } else {
            MPI_Recv(&in_val, 1, MPI_INT, partner, 0, comm, 
                  MPI_STATUS_IGNORE); 
         }
      }
      bitmask <<= 1;
      participate <<= 1;
   }
   return in_val;

}  /* Bcast */