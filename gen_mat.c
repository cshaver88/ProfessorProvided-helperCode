/* File:     gen_mat.c
 * Purpose:  Generate a random adjacency matrix for use in Floyd's algorithm.
 *
 * Input:    None
 * Output:   The number of vertices and the adjacency matrix
 *
 * Compile:  gcc -g -Wall -o gen_mat gen_mat.c
 * Run:      ./gen_mat <number of vertices>
 * 
 * Notes:
 * 1.  Max edge cost is MAX_COST - 1 
 * 2.  Diagonal entries are 0
 * 3.  For a given n, the matrix generated will always be the same.
 * 4.  There's no guarantee that the graph is strongly connected:  there
 *     may be a pair of vertices i and j for which there's no path 
 *     i -> j.
 */

#include <stdio.h>
#include <stdlib.h>

const int INFINITY = 1000000;
const int MAX_COST = 10;

void Usage(char* prog_name);

int main(int argc, char* argv[]) {
   int n, i, j, val;

   if (argc != 2) Usage(argv[0]);
   n = strtol(argv[1], NULL, 10);

   printf("%d\n", n);
   for (i = 0; i < n; i++) {
      for (j = 0; j < n; j++)
         if (i == j)
            printf("0 ");
         else {
            val = random() % MAX_COST + 1;
            if (val == MAX_COST)
               printf("%d ", INFINITY);
            else
               printf("%d ", val);
         }
      printf("\n");
   }
         
   return 0;
}  /* main */

void Usage(char* prog_name) {
   fprintf(stderr, "usage:  %s <number of rows>\n", prog_name);
   exit(0);
}  /* Usage */