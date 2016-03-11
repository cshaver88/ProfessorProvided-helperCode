/* File:      gs_mpi_allred.c
 * Purpose:   Driver for MPI_Allreduce
 * Input:     None
 * Output:    Random values generated by processes and sum of these
 *            values.
 *
 * Algorithm: 
 *    1. Each process computes a random int x from 0 to MAX_CONTRIB-1
 *    2. Processes call MPI_Allreduce function which sums the local
 *       values on all the processes
 *    3. Each process prints global sum
 *
 * Compile:  mpicc -g -Wall -o gs_mpi_allred gs_mpi_allred.c
 * Run:      mpiexec -n <number of processes> ./gs_mpi_allred
 *
 * Notes:
 *    1. This version has no restriction on the number of processes.
 *    2. The value returned by MPI_Reduce on processes other
 *       than 0 is meaningless.
 */
#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define MAX_CONTRIB 20

int main(void) {
   int p, my_rank;
   MPI_Comm comm;
   int x;
   int total;

   MPI_Init(NULL, NULL);
   comm = MPI_COMM_WORLD;
   MPI_Comm_size(comm, &p);
   MPI_Comm_rank(comm, &my_rank);

   srandom(my_rank+1);
   x = random() % MAX_CONTRIB;
   printf("Proc %d > x = %d\n", my_rank, x);

   MPI_Allreduce(&x, &total, 1, MPI_INT, MPI_SUM, comm);

   printf("Proc %d > The total is %d\n", my_rank, total);

   MPI_Finalize();
   return 0;
}  /* main */