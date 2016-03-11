/* File:     arg_passing2.c
 * Purpose:  Illustrate argument passing in C.  This version
 *           simulates pass-by-reference.
 *
 * Input:    Three ints
 * Output:   Values of variables at various points in the program.
 *
 * Compile:  Using Linux and gcc
 *           gcc -g -Wall -o arg_passing2 arg_passing2.c
 * Run:      ./arg_passing2
 */

#include <stdio.h>

void Pass_by_reference(int* x_p, int* y_p, int* z_p);

/*------------------------------------------------------------*/
int main(void) {
   int x, y, z;

   printf("Enter three ints\n");
   scanf("%d%d%d", &x, &y, &z);
   printf("In main, x = %d, y = %d, z = %d\n\n", x, y, z);

   Pass_by_reference(&x, &y, &z);

   printf("In main, x = %d, y = %d, z = %d\n", x, y, z);

   return 0;
}  /* main */


/*------------------------------------------------------------*/
void Pass_by_reference(int* x_p, int* y_p, int* z_p) {
   printf("In Pass_by_reference, x = %d, y = %d, z = %d\n", 
      *x_p, *y_p, *z_p);
   *x_p = 2 + *x_p;
   *y_p = *y_p - 3;
   *z_p = 7 + *z_p;
   printf("In Pass_by_reference, x = %d, y = %d, z = %d\n\n", 
      *x_p, *y_p, *z_p);
}  /* Pass_by_reference */