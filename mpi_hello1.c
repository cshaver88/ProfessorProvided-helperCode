/* File:       mpi_hello1.c
 *
 * Purpose:    A "hello,world" program that uses MPI
 *
 * Compile:    mpicc -g -Wall -o mpi_hello mpi_hello.c
 * Run:        mpiexec -n<number of processes> mpi_hello
 *
 * Input:      None
 * Output:     A greeting from each process
 *
 * Algorithm:  Each process sends a message to the "next
 *             higher ranked process" which prints the message 
 *             it has received.
 */
#include <stdio.h>
#include <string.h>  /* For strlen             */
#include <mpi.h>     /* For MPI functions, etc */

const int MAX_STRING = 100;

int main(void) {
   char       greeting[MAX_STRING];
   int        my_rank, p;
   int        src, dest;

   /* Start up MPI */
   MPI_Init(NULL, NULL);

   /* Get the number of processes */
   MPI_Comm_size(MPI_COMM_WORLD, &p);

   /* Get my rank among all the processes */
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

   dest = (my_rank + 1) % p;
   src = (my_rank + p - 1) % p;

   sprintf(greeting, "Greetings to %d from process %d of %d", 
      dest, my_rank, p);
   if (p > 1) {
      MPI_Send(greeting, strlen(greeting)+1, MPI_CHAR, dest, 0,
         MPI_COMM_WORLD);
      MPI_Recv(greeting, MAX_STRING, MPI_CHAR, src, 
         0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
   }
   printf("Proc %d > %s\n", my_rank, greeting);

   /* Shut down MPI */
   MPI_Finalize();
   return 0;
}  /* main */