/* File:     structs3a.c
 *
 * Purpose:  Determine whether structs are passed by reference
 * Input:    None
 * Output:   struct member values
 *
 * Compile:  gcc -g -Wall -o structs3a structs3a.c
 * Usage:    ./structs3a
 */
#include <stdio.h>

typedef struct {
   double value;
   int    row;
   int    col;
} element_t;  

void pass_struct(element_t x);

int main(void) {
   element_t element;

   element.value = 5.5;
   element.row = 6;
   element.col = 10;

   printf("Before call to pass_struct\n");
   printf("element.value = %f\n", element.value);
   printf("element.row   = %d\n", element.row);
   printf("element.col   = %d\n", element.col);

   pass_struct(element);

   printf("After call to pass_struct\n");
   printf("element.value = %f\n", element.value);
   printf("element.row   = %d\n", element.row);
   printf("element.col   = %d\n", element.col);

   return 0;
}  /* main */

void pass_struct(element_t x) {

   printf("\nIn pass_struct\n");
   printf("x.value = %f\n", x.value);
   printf("x.row   = %d\n", x.row);
   printf("x.col   = %d\n", x.col);

   x.value = 2.71828;
   x.row = 43;
   x.col = 1027;

   printf("\nIn pass_struct\n");
   printf("x.value = %f\n", x.value);
   printf("x.row   = %d\n", x.row);
   printf("x.col   = %d\n\n", x.col);
}  /* pass_struct */