/* File:     floyd.c
 * Purpose:  Implement Floyd's algorithm for solving the all-pairs shortest
 *           path problem:  find the length of the shortest path between each
 *           pair of vertices in a directed graph.
 *
 * Input:    n, the number of vertices in the digraph
 *           mat, the adjacency matrix of the digraph
 * Output:   A matrix showing the costs of the shortest paths
 *
 * Compile:  gcc -g -Wall -o floyd floyd.c
 *           (See note 7)
 * Run:      ./floyd
 *           For large matrices, put the matrix into a file with n as
 *           the first line and run with ./floyd < large_matrix
 *
 * Notes:
 * 1.  The input matrix is overwritten by the matrix of lengths of shortest
 *     paths.
 * 2.  Edge lengths should be nonnegative.
 * 3.  If there is no edge between two vertices, the length is the constant
 *     INFINITY.  So input edge length should be substantially less than
 *     this constant.
 * 4.  The cost of travelling from a vertex to itself is 0.  So the adjacency
 *     matrix has zeroes on the main diagonal.
 * 5.  No error checking is done on the input.
 * 6.  The adjacency matrix is stored as a 1-dimensional array and subscripts
 *     are computed using the formula:  the entry in the ith row and jth
 *     column is mat[i*n + j]
 * 7.  Use the compile flag -DSHOW_INT_MATS to print the matrix after its
 *     been updated with each intermediate city.
 */
#include <stdio.h>
#include <stdlib.h>

const int INFINITY = 1000000;

void Read_matrix(int mat[], int n);
void Print_matrix(int mat[], int n);
void Floyd(int mat[], int n);

int main(void) {
   int  n;
   int* mat;

   printf("How many vertices?\n");
   scanf("%d", &n);
   mat = malloc(n*n*sizeof(int));

   printf("Enter the matrix\n");
   Read_matrix(mat, n);

   Floyd(mat, n);

   printf("The solution is:\n");
   Print_matrix(mat, n);

   free(mat);

   return 0;
}  /* main */

/*-------------------------------------------------------------------
 * Function:  Read_matrix
 * Purpose:   Read in the adjacency matrix
 * In arg:    n
 * Out arg:   mat
 */
void Read_matrix(int mat[], int n) {
   int i, j;

   for (i = 0; i < n; i++)
      for (j = 0; j < n; j++)
         scanf("%d", &mat[i*n+j]);
}  /* Read_matrix */

/*-------------------------------------------------------------------
 * Function:  Print_matrix
 * Purpose:   Print the contents of the matrix
 * In args:   mat, n
 */
void Print_matrix(int mat[], int n) {
   int i, j;

   for (i = 0; i < n; i++) {
      for (j = 0; j < n; j++)
         if (mat[i*n+j] == INFINITY)
            printf("i ");
         else
            printf("%d ", mat[i*n+j]);
      printf("\n");
   }
}  /* Print_matrix */

/*-------------------------------------------------------------------
 * Function:    Floyd
 * Purpose:     Apply Floyd's algorithm to the matrix mat
 * In arg:      n
 * In/out arg:  mat:  on input, the adjacency matrix, on output
 *              lengths of the shortest paths between each pair of
 *              vertices.
 */
void Floyd(int mat[], int n) {
   int int_city, city1, city2, temp;

   for (int_city = 0; int_city < n; int_city++) {
      for (city1 = 0; city1 < n; city1++)
         for (city2 = 0; city2 < n; city2++) {
               temp = mat[city1*n + int_city] + mat[int_city*n + city2];
               if (temp < mat[city1*n+city2])
                  mat[city1*n + city2] = temp;
         }
#     ifdef SHOW_INT_MATS
      printf("After int_city = %d\n", int_city);
      Print_matrix(mat, n);
#     endif
   }
}  /* Floyd */