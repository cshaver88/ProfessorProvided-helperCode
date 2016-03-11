/* File:     structs4.c
 *
 * Purpose:  Illustrate pass by reference of structs using second solution 
 *           to problem of passing struct args -- using typedef
 * Input:    None
 * Output:   struct member values
 *
 * Compile:  gcc -g -Wall -o structs4 structs4.c
 * Usage:    ./structs4
 *
 * Note:     This program has a few bugs . . .
 */
#include <stdio.h>

typedef struct {
   double value;
   int    row;
   int    col;
} element_t;  

void pass_struct(element_t* x_p);

int main(void) {
   element_t element;

   element.value = 5.5;
   element.row = 6;
   element.col = 10;

   printf("element.value = %f\n", element.value);
   printf("element.row   = %d\n", element.row);
   printf("element.col   = %d\n", element.col);

   pass_struct(&element);

   printf("\nelement.value = %f\n", element.value);
   printf("element.row   = %d\n", element.row);
   printf("element.col   = %d\n", element.col);

   return 0;
}  /* main */

void pass_struct(element_t* x_p) {
   printf("\nIn pass_struct\n");
   printf("x.value = %f\n", (*x_p).value);
   printf("x.row   = %d\n", (*x_p).row);
   printf("x.col   = %d\n", (*x_p).col);

   *x_p.value = 1.2;
   *x_p.row = 10;
   *x_p.col = 16;

   printf("\nIn pass_struct\n");
   printf("x.value = %f\n", (*x_p).value);
   printf("x.row   = %d\n", (*x_p).row);
   printf("x.col   = %d\n", (*x_p).col);

}  /* pass_struct */