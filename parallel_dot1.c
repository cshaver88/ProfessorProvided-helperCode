/* File:     parallel_dot1.c 
 * Purpose:  compute a dot product of a vector distributed among
 *           the processes.  Uses a block distribution of the vectors.
 *
 * Compile:  mpicc -g -Wall -o parallel_dot1 parallel_dot1.c
 * Run:      mpiexec -n <number of processes> ./parallel_dot1
 *
 * Input:    n: global order of vectors
 *           x, y:  the vectors
 * Output:   the dot product of x and y.
 *
 * Notes:  
 *     1.  n, the global order of the vectors must be divisible by p, 
 *         the number of processes.
 *     2.  Result returned by Parallel_dot is valid on all processes
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void   Read_vector(char* prompt, float local_v[], int local_n, int n,
          int my_rank, MPI_Comm comm);
void   Print_results(float dot, int p, int my_rank, MPI_Comm comm);
float  Serial_dot(float x[], float y[], int m);
float  Parallel_dot(float local_x[], float local_y[], int local_n,
          MPI_Comm comm);
    
int main(void) {
    float*   local_x;
    float*   local_y;
    int      n;
    int      local_n;  /* = n/p */
    float    dot;
    int      p;
    int      my_rank;
    MPI_Comm comm;

    MPI_Init(NULL, NULL);
    comm = MPI_COMM_WORLD;
    MPI_Comm_size(comm, &p);
    MPI_Comm_rank(comm, &my_rank);

    if (my_rank == 0) {
        printf("Enter the order of the vectors\n");
        scanf("%d", &n);
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    local_n = n/p;

    local_x = malloc(local_n*sizeof(float));
    local_y = malloc(local_n*sizeof(float));
    Read_vector("the first vector", local_x, local_n, n, my_rank, comm);
    Read_vector("the second vector", local_y, local_n, n, my_rank, comm);

    dot = Parallel_dot(local_x, local_y, local_n, comm);

    Print_results(dot, p, my_rank, comm);

    free(local_x);
    free(local_y);
    MPI_Finalize();

    return 0;
}  /* main */
   

/*---------------------------------------------------------------
 * Function: Read_vector
 * Purpose:  Read a vector from stdin and distributed it by blocks
 *           among the processes
 * In args:  prompt:  prompt user for input
 *           local_n:  size of this process' piece
 *           n:  size of global vector
 *           my_rank, comm:  usual MPI variables
 * Out arg:  local_v:  this process' piece of the vector
 */

void Read_vector(
         char*    prompt     /* in  */,
         float    local_v[]  /* out */,
         int      local_n      /* in  */,
         int      n          /* in  */,
         int      my_rank    /* in  */,
         MPI_Comm comm       /* in  */) {
    int    i;
    float* temp;

    if (my_rank == 0) {
        temp = malloc(n*sizeof(float));
        printf("Enter %s\n", prompt);
        for (i = 0; i < n; i++)
            scanf("%f", &temp[i]);
        MPI_Scatter(temp, local_n, MPI_FLOAT, local_v, local_n, MPI_FLOAT,
           0, comm);
        free(temp);
    } else {
        MPI_Scatter(temp, local_n, MPI_FLOAT, local_v, local_n, MPI_FLOAT,
           0, comm);
    }
}  /* Read_vector */

/*---------------------------------------------------------------
 * Function:  Print_results
 * Purpose:   Print each process' value in dot
 * In args:   dot:  local dot product
 *            p, my_rank comm:  usual MPI variables
 */
void Print_results(
          float    dot     /* in */,
          int      p       /* in */,
          int      my_rank /* in */,
          MPI_Comm comm    /* in */) {
   int    q;
   float* temp;

   if (my_rank == 0) {
      temp = malloc(p*sizeof(float));
      MPI_Gather(&dot, 1, MPI_FLOAT, temp, 1, MPI_FLOAT, 0, comm);
      for (q = 0; q < p; q++)
         printf("Proc %d > dot = %f\n", q, temp[q]);
      free(temp);
   } else {
      MPI_Gather(&dot, 1, MPI_FLOAT, temp, 1, MPI_FLOAT, 0, comm);
   }
}  /* Print_results */

/*---------------------------------------------------------------
 * Function:  Serial_dot
 * Purpose:   Compute a dot product of two local vectors
 * In args:   x:  local vector
 *            y:  local vector
 *            n:  number of components in x and y
 * Ret val:   dot product of local vectors x and y
 */

float Serial_dot(
          float  x[]  /* in */, 
          float  y[]  /* in */, 
          int    n    /* in */) {

    int    i; 
    float  sum = 0.0;

    for (i = 0; i < n; i++)
        sum += x[i]*y[i];
    return sum;
} /* Serial_dot */


/*---------------------------------------------------------------
 * Function:  Parallel_dot
 * Purpose:   Compute a dot product of two distributed vectors
 * In args:   local_x:  this process' piece of the first dist vect
 *            local_y:  this process' piece of the second dist vect
 *            local_n:  number of components in x, y
 *            comm:     communicator for MPI_Allreduce
 * Ret val:   global dot product of the distributed vectors
 * Note:      This version returns the result to all the processes
 */

float Parallel_dot(
          float    local_x[]  /* in */,
          float    local_y[]  /* in */,
          int      local_n    /* in */,
          MPI_Comm comm       /* in */) {

    float  local_dot;
    float  dot = 0.0;

    local_dot = Serial_dot(local_x, local_y, local_n);
    MPI_Allreduce(&local_dot, &dot, 1, MPI_FLOAT, MPI_SUM, comm);
    return dot;
} /* Parallel_dot */