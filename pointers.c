/* File:     pointers.c
 * Purpose:  Show that pointers are addresses, and an illegal address
 *           in a pointer can cause a program to crash when it's
 *           dereferenced or followed
 *
 * Compile:  gcc -g -Wall -o pointers pointers.c
 * Run:      ./pointers
 *
 * Input:    A long hexadecimal int
 * Output:   Values of x_p and *x_p
 */
#include <stdio.h>

int main(void) {
   int x = 3;
   int* x_p;
   long z;

   /* Legal assignment to x_p */
   x_p = &x;
   printf("*x_p = %d\n", *x_p);
   /* Print the address stored in x_p */
   printf("x_p = %p\n", x_p);

   /* Use long hex format for z */
   scanf("%lx", &z); 

   /* The cast will allow us to assign an arbitrary long to x_p */
   x_p = (int*) z;
   printf("x_p = %p", x_p);
   /* This guarantees that x_p will be printed even if the next statement
    * crashes the program */
   fflush(stdout);

   /* If the address in x_p is illegal, this will crash the program */
   printf("*x_p = %d\n", *x_p);
   return 0;
}  /* main */