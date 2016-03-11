/* File:     parallel_dot.c 
 * Purpose:  compute a dot product of a vector distributed among
 *           the processes.  Uses a block distribution of the vectors.
 *
 * Compile:  mpicc -g -Wall -o parallel_dot parallel_dot.c
 * Run:      mpiexec -n <number of processes> ./parallel_dot
 *
 * Input:    n: global order of vectors
 *           x, y:  the vectors
 * Output:   the dot product of x and y.
 *
 * Notes:  
 *     1.  n, the global order of the vectors must be divisible by p, 
 *         the number of processes.
 *     2.  Result returned by Parallel_dot is valid only on process 0
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void   Read_vector(char* prompt, float local_v[], int local_n, int n,
          int my_rank, MPI_Comm comm);
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

    if (my_rank == 0)
        printf("The dot product is %f\n", dot);

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
         int      local_n    /* in  */,
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
 *            comm:     communicator for MPI_Reduce
 * Ret val:   global dot product of the distributed vectors
 * Note:      This version returns the result only on process 0
 */
float Parallel_dot(
          float    local_x[]  /* in */,
          float    local_y[]  /* in */,
          int      local_n    /* in */,
          MPI_Comm comm       /* in */) {

    float  local_dot;
    float  dot = 0.0;

    local_dot = Serial_dot(local_x, local_y, local_n);
    MPI_Reduce(&local_dot, &dot, 1, MPI_FLOAT, MPI_SUM, 0, comm);
    return dot;
} /* Parallel_dot */