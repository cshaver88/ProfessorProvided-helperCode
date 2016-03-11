/* File:     seg_fault.c
 * Purpose:  Illustrate the fact that C doesn't check array bounds
 *
 * Compile:  gcc -g -Wall -o seg_fault seg_fault.c
 * Run:      ./seg_fault
 *
 * Input:    A sequence of subscripts (possibly outside the specified
 *           range).  A negative value to terminate.
 * Output:   Values of the array elements when the subscript is valid.
 *           Junk or crash when the subscript is invalid.
 */
#include <stdio.h>

const int MAX_ARRAY = 5;

int main(void) {
   int array[MAX_ARRAY];  /* Valid subscripts 0-4 */
   int i;

   for (i = 0; i < MAX_ARRAY; i++)
      array[i] = 2*i + 1;

   for (i = 0; i < MAX_ARRAY; i++)
      printf("array[%d] = %d\n", i, array[i]);

   printf("Next subscript?\n");
   scanf("%d", &i);
   while (i >= 0) {
      printf("array[%d] = ", i);
      fflush(stdout);
      printf("%d\n\n", array[i]);
      printf("Next subscript?\n");
      scanf("%d", &i);
   }

   return 0;
}  /* main */