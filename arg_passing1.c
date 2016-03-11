/* File:     arg_passing1.c
 * Purpose:  Illustrate argument passing in C
 *
 * Input:    Three ints
 * Output:   Values of variables at various points in the program.
 *
 * Compile:  Using Linux and gcc
 *           gcc -g -Wall -o arg_passing1 arg_passing1.c
 * Run:      ./arg_passing1
 */

#include <stdio.h>

void Pass_by_value(int x, int y, int z);

/*------------------------------------------------------------*/
int main(void) {
   int x, y, z;

   printf("Enter three ints\n");
   scanf("%d%d%d", &x, &y, &z);
   printf("In main, x = %d, y = %d, z = %d\n\n", x, y, z);

   Pass_by_value(x, y, z);

   printf("In main, x = %d, y = %d, z = %d\n", x, y, z);

   return 0;
}  /* main */


/*------------------------------------------------------------*/
void Pass_by_value(int x, int y, int z) {
   printf("In Pass_by_value, x = %d, y = %d, z = %d\n", x, y, z);
   x = 2 + x;
   y = y - 3;
   z = 7 + z;
   printf("In Pass_by_value, x = %d, y = %d, z = %d\n\n", x, y, z);
}  /* Pass_by_value */