/* File:     mpi_primes_sort.c
 * Purpose:  Find all primes less than or equal to an input value 
 *
 * Input:    n:  integer >= 2 (from command line)
 * Output:   Sorted list of primes between 2 and n,
 *
 * Compile:  mpicc -g -Wall -o mpi_primes_sort mpi_primes_sort.c -lm
 * Usage:    mpiexec -n <p> ./mpi_primes_sort <n>
 *           p:  number of MPI processes
 *           n:  max int to test for primality
 *
 * Note:
 * 1.  DEBUG compile flag for verbose output
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <mpi.h>

const int STRING_MAX = 1000;

int Get_n(int argc, char* argv[], int my_rank, int p, MPI_Comm comm);
int Is_prime(int i);
void Print_primes(int my_primes[], int my_prime_count, int my_rank,
      int p, MPI_Comm comm);
void Merge_lists(int my_contrib[], int my_count, int** list_p, 
   int* size_p, int my_rank, int p, MPI_Comm comm);
void Merge(int** a_p, int* asize_p, int b[], int bsize, int** c_p);
void Compute_list_sizes(int prime_counts[], int recv_counts[], int p);
void Print_list(char* title, int list[], int n, int my_rank);

int main(int argc, char* argv[]) {
   int n, i, p, local_n, incr;
   int my_rank;   // my process rank
   MPI_Comm comm;
   int* my_primes, my_prime_count=0;

   MPI_Init(&argc, &argv);
   comm = MPI_COMM_WORLD;
   MPI_Comm_size(comm, &p);
   MPI_Comm_rank(comm, &my_rank);

   n = Get_n(argc, argv, my_rank, p, comm);
   local_n = n/(2*p)+2;
   my_primes = malloc(local_n*sizeof(int));

   if (my_rank == 0) my_primes[my_prime_count++] = 2;
   incr = 2*p;
   for (i = 2*my_rank + 3; i <= n; i += incr)
      if (Is_prime(i)) {
         my_primes[my_prime_count++] = i;
#        ifdef DEBUG
         printf("Proc %d > %d\n", my_rank, i);
#        endif
      }
   Print_list("After prime finder", my_primes, my_prime_count, my_rank);

   Print_primes(my_primes, my_prime_count, my_rank, p, comm);

   free(my_primes);

   MPI_Finalize();
   return 0;
}  /* main */

/*-------------------------------------------------------------------
 * Function:    Get_n
 * Purpose:     Get the input value n
 * Input args:  my_rank:  process rank in comm
 *              p:  number of processes in comm
 *              comm:  communicator used by program
 *              argc:  number of command line args
 *              argv:  array of command line args
 */
int Get_n(int argc, char* argv[], int my_rank, int p,  MPI_Comm comm) {
   int n;

   if (my_rank == 0) {
      if (argc != 2)
         n = -1;  // error
      else {
         n = strtol(argv[1], NULL, 10);
      }
   } 
   MPI_Bcast(&n, 1, MPI_INT, 0, comm);

   /* Check for bogus input */
   if (n <= 1) {
      if (my_rank == 0) {
         fprintf(stderr, "usage: mpiexec -n <p> %s <n>\n", argv[0]);
         fprintf(stderr, "   p = number of MPI processes\n");
         fprintf(stderr, "   n = max integer to test for primality (>= 2)\n");
      }
      MPI_Finalize();
      exit(0);
   }
   return n;

}  /* Get_n */

/*-------------------------------------------------------------------
 * Function:   Is_prime
 * Purpose:    Determine whether the argument is prime
 * Input arg:  i
 * Return val: true (nonzero) if arg is prime, false (zero) otherwise
 */
int Is_prime(int i) {
   int j;

   for (j = 2; j <= sqrt(i); j++)
      if (i % j == 0)
         return 0;
   return 1;
}  /* Is_prime */


/*-------------------------------------------------------------------
 * Function:    Print_primes
 * Purpose:     Print the primes found by the processes
 * Input args:  my_primes: the primes found by the current process
 *              my_prime_count:  the number of primes found by the
 *                 current process
 *              my_rank, p, comm:  the usual MPI variables.
 */
void Print_primes(int my_primes[], int my_prime_count, int my_rank,
      int p, MPI_Comm comm) {
   int* all_primes;
   int all_primes_count, i;

   Merge_lists(my_primes, my_prime_count, &all_primes, &all_primes_count,
         my_rank, p, comm);

   if (my_rank == 0) {
      printf("The primes are\n");
      for (i = 0; i < all_primes_count; i++)
         printf("%d ", all_primes[i]);
      printf("\n");
      free(all_primes);
   }
   
}  /* Print_primes */


/*-------------------------------------------------------------------
 * Function:  Merge_lists
 * Purpose:   Merge a collection of sorted lists, one per process.
 * In args:   my_contrib:  my sorted list
 *            my_count:  the number of elements in my sorted list
 *            my_rank, p, comm: the usual MPI variables
 * Out args:  list_p:  the merged list
 *            size_p: number of elements in the merged list
 *
 */
void Merge_lists(int my_contrib[], int my_count, int** list_p, 
   int* size_p, int my_rank, int p, MPI_Comm comm) {

   int my_size, my_recv_size, done = 0, partner, i, curr_size;
   int* my_list = NULL, *recv_list = NULL, *temp = NULL;
   unsigned bitmask = 1;
   int *counts = malloc(p*sizeof(int));
   int *recv_counts = malloc(p*sizeof(int));
   MPI_Status status;  /* Needed by valgrind wrappers */
   int recv_count;     /* Used during debugging */

   MPI_Allgather(&my_count, 1, MPI_INT, counts, 1, MPI_INT, comm);
   Print_list("list sizes", counts, p, my_rank);

#  ifdef DEBUG
   if (my_rank == 0) {
      printf("Counts after allgather:\n");
      for (i = 0; i < p; i++)
         printf("%d ", counts[i]);
      printf("\n");
   }
#  endif
   Compute_list_sizes(counts, recv_counts, p);
#  ifdef DEBUG
   if (my_rank == 0) {
      printf("Counts after Compute_list_sizes:\n");
      for (i = 0; i < p; i++)
         printf("%d ", counts[i]);
      printf("\n");
      printf("Recv_counts after Compute_list_sizes:\n");
      for (i = 0; i < p; i++)
         printf("%d ", recv_counts[i]);
      printf("\n");
   }
#  endif
   Print_list("recv counts", recv_counts, p, my_rank);

   my_size = counts[my_rank];
   if (my_size > 0) my_list = malloc(my_size*sizeof(int));
   if (my_size > 0) temp = malloc(my_size*sizeof(int));
   my_recv_size = recv_counts[my_rank];
   if (my_recv_size > 0) recv_list = malloc(my_recv_size*sizeof(int));
   for (i = 0; i < my_count; i++)
      my_list[i] = my_contrib[i];
   curr_size = my_count;

   while (!done && bitmask < p) {
      partner = my_rank ^ bitmask;
      if (my_rank < partner && counts[partner] > 0) {
         if (partner < p) {
            MPI_Recv(recv_list, counts[partner], MPI_INT,
               partner, 0, comm, &status);
//          MPI_Recv(recv_list, my_recv_size, MPI_INT,
//             partner, 0, comm, &status);
//          MPI_Get_count(&status, MPI_INT, &recv_count);
//          for (i = curr_size; i < curr_size + counts[partner]; i++)
//             my_list[i] = recv_list[i-curr_size];
//          curr_size += counts[partner];
//          Merge(&my_list, &curr_size, recv_list, counts[partner],
//                &temp);
            recv_count = counts[partner];
#           ifdef DEBUG
            printf("Proc %d > received %d from %d, predicted %d\n",
                  my_rank, recv_count, partner, counts[partner]);
            fflush(stdout);
#           endif
            Merge(&my_list, &curr_size, recv_list, recv_count,
                  &temp);
            Print_list("after merge", my_list, curr_size, my_rank);
         }
         bitmask <<= 1;
      } else {
#        ifdef DEBUG
         printf("Proc %d > send to = %d, bitmask = %d, my_size = %d\n",
               my_rank, partner, bitmask, my_size);
#        endif
         MPI_Send(my_list, my_size, MPI_INT, partner, 0, comm);
         done = 1;
      }
   }

   if (my_rank == 0) {
      *list_p = my_list;
      *size_p = my_size;
   } else if (my_list != NULL)
      free(my_list);
   if (recv_list != NULL) free(recv_list);
   if (temp != NULL) free(temp);

} /* Merge_lists */

/*-------------------------------------------------------------------
 * Function:    Merge
 * Purpose:     Merge two sorted lists
 * In/out arg:  a:  on input one of the sorted lists
 *                  on return the combined list
 *              asize_p:  on input number of elements in a
 *                  on return number of elements in updated a
 * In args:     b:  the other input list
 *              bsize:  the number of elements in b
 *              c:  temporary storage for the merged list
 */
void Merge(int** a_p, int* asize_p, int b[], int bsize, int** c_p) {
   int *a = *a_p;
   int *c = *c_p;
   int ai, bi, ci;

   ai = bi = ci = 0;
   while (ai < *asize_p && bi < bsize) 
      if (a[ai] <= b[bi]) 
         c[ci++] = a[ai++];
      else
         c[ci++] = b[bi++];
   while (ai < *asize_p)
      c[ci++] = a[ai++];
   while (bi < bsize)
      c[ci++] = b[bi++];

// memcpy(a, c, ci*sizeof(int));
   *a_p = c;
   *c_p = a;
   *asize_p = ci;
}  /* Merge */

/*-------------------------------------------------------------------
 * Function:    Compute_list_sizes
 * Purpose:     Find the sizes of the lists of primes each process
 *              will ultimately store.  Also compute the maximum
 *              size list each process will receive in any one
 *              call to MPI_Recv.
 * Input args:  prime_counts: the qth entry is the number of primes 
 *                 found by process q
 *              p:  the number of processes and the size of prime_counts
 */
void Compute_list_sizes(int prime_counts[], int recv_counts[], int p) {
   unsigned bitmask;
   int partner, rank, incr;
#  ifdef LIST_SIZE_DEBUG
   int my_rank;

   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
#  endif

   for (rank = 0; rank < p; rank++) recv_counts[rank] = 0;

   for (bitmask = 1, incr = 2; bitmask < p; bitmask <<= 1, incr *= 2) {
      for (rank = 0; rank < p; rank += incr) {
         partner = rank ^ bitmask;
         if (partner < p) {
            prime_counts[rank] += prime_counts[partner];
            if (recv_counts[rank] < prime_counts[partner])
               recv_counts[rank] = prime_counts[partner];
         }
      }
#     ifdef LIST_SIZE_DEBUG
      if (my_rank == 0) {
         printf("After bitmask = %d, prime_counts =\n", bitmask);
         for (rank = 0; rank < p; rank++)
            printf("%d ", prime_counts[rank]);
         printf("\n");
         printf("After bitmask = %d, recv_counts =\n", bitmask);
         for (rank = 0; rank < p; rank++)
            printf("%d ", recv_counts[rank]);
         printf("\n");
      }
#     endif
   }
            
}  /* Compute_list_size */   

/*-------------------------------------------------------------------
 * Function:  Print_list
 * Purpose:   Convert a list of ints to a single string before
 *            printing.  This should make it less likely that the
 *            output is interrupted by another process.  This is
 *            mainly intended for debugging purposes.
 * In args:   title:  title of list
 *            list:  the ints to be printed
 *            n:  the number of ints
 *            my_rank:  the usual MPI variable
 */
void Print_list(char* title, int list[], int n, int my_rank) {
   char string[STRING_MAX];
   char* s_p;
   int i;

   sprintf(string, "Proc %d %s > ", my_rank, title);
   // Pointer arithmetic:  make s_p point to the character strlen(string)
   // into string; i.e., make it point at the `\0'
   s_p = string + strlen(string);
   for (i = 0; i < n; i++) {
      sprintf(s_p, "%d ", list[i]);
      s_p = string + strlen(string);
   }

   printf("%s\n", string);
   fflush(stdout);
}  /* Print_list */