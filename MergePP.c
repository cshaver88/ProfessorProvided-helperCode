/* File:     Merge.c
 * Purpose:  Merge two sorted arrays of ints
 *
 * Compile:  gcc -g -Wall -o merge merge.c
 * Run:      ./merge
 *
 * Input:
 *    asize, bsize:  number of elements in input arrays
 *    A, B:  the two sorted arrays to be Merged
 * Output:
 *    C:  the merged array
 *
 * Notes:
 *    1. Maximum array size is defined in global constant MAX
 *    2. There's no check on input array sizes
 *    3. There's no check that the input arrays are sorted
 */
#include <stdio.h>

/* This is somewhat equivalent to a final variable in Java */
const int MAX = 100;

void Read_array(int A[], int n);
void Print_array(int A[], int n);
void Merge(int A[], int asize, int B[], int bsize, int C[], int csize);

/*-------------------------------------------------------------------*/
int main(void) {
   int A[MAX];
   int B[MAX];
   int C[MAX];
   int asize, bsize, csize;

   printf("How many elements in A? B?\n");
   scanf("%d %d", &asize, &bsize);

   printf("Enter the elements of A.\n");
   Read_array(A, asize);

   printf("Enter the elements of B.\n");
   Read_array(B, bsize);
   
   csize = asize + bsize;
   Merge(A, asize, B, bsize, C, csize);
   
   printf("C = \n");
   Print_array(C, csize);
   return 0;
}  /* main */


/*-------------------------------------------------------------------
 * Function:   Read_array
 * Purpose:    read in the contents of the array A
 * Input arg:  n, the number of elements to be read
 * Output arg: A, the array
 */
void Read_array(int A[], int n) {
   int i;
   for (i = 0; i < n; i++)
      scanf("%d", &A[i]);
}  /* Read_array */


/*-------------------------------------------------------------------
 * Function:   Print_array
 * Purpose:    print the contents of the array A
 * Input args:
 *    n, the number of elements to be read
 *    A, the array
 */
void Print_array(int A[], int n) {
   int i;
   for (i = 0; i < n; i++)
      printf("%d ", A[i]);
   printf("\n");
}  /* Print_array */


/*-------------------------------------------------------------------
 * Function:   Merge
 * Purpose:    Merge the contents of the arrays A and B into array C
 * Input args:
 *    asize:  the number of elements in A
 *    bsize:  the number of elements in B
 *    csize:  the number of elements in C (= asize + bsize)
 *    A, B:  the arrays
 *
 * Output arg:
 *    C:  result array
 */
void Merge(int A[], int asize, int B[], int bsize, int C[], int csize) {
   int ai, bi, ci;
   
   ai = bi = ci = 0;
   while (ai < asize && bi < bsize) {
      if (A[ai] <= B[bi]) {
         C[ci] = A[ai];
         ci++; ai++;
      } else {
         C[ci] = B[bi];
         ci++; bi++;
      }
   }

   if (ai >= asize)
      for (; ci < csize; ci++, bi++)
         C[ci] = B[bi];
   else
      for (; ci < csize; ci++, ai++)
         C[ci] = A[ai];
}  /* Merge */