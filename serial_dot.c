/* File:     serial_dot.c 
 * Purpose:  compute a dot product on a single processor.
 *
 * Input:    n: order of vectors
 *           x, y:  the vectors
 *
 * Output:   the dot product of x and y.
 *
 * Compile:  gcc -g -Wall -o serial_dot serial_dot.c
 * Run:      ./serial_dot
 *
 */
#include <stdio.h>
#include <stdlib.h>

void Read_vector(char* prompt, float v[], int n);
float Serial_dot(float x[], float y[], int n);
    
int main(void) {
    float* x;
    float* y;
    int    n;
    float  dot;

    printf("Enter the order of the vectors\n");
    scanf("%d", &n);
    x = malloc(n*sizeof(float));
    y = malloc(n*sizeof(float));
    Read_vector("the first vector", x, n);
    Read_vector("the second vector", y, n);

    dot = Serial_dot(x, y, n);
    printf("The dot product is %f\n", dot);

    free(x);
    free(y);
    return 0;
}  /* main */
   

/*---------------------------------------------------------------
 * Function:  Read_vector
 * Purpose:   Read a vector from stdin
 * In arg:    n:  number of components
 * Out arg:   v:  the vector
 */
void Read_vector(
         char*  prompt  /* in  */,
         float  v[]     /* out */,
         int    n       /* in  */) {
    int i;

    printf("Enter %s\n", prompt);
    for (i = 0; i < n; i++)
        scanf("%f", &v[i]);
}  /* Read_vector */


/*---------------------------------------------------------------
 * Function:  Serial_dot
 * Purpose:   Find the dot product of two vectors
 * In args:   x, y:  the two vectors
 *            n:  the number of components in the two vectors
 * Ret val:   the dot product of x and y
 */
float Serial_dot(
          float  x[]  /* in */, 
          float  y[]  /* in */, 
          int    n    /* in */) {

    int    i; 
    float  sum = 0.0;

    for (i = 0; i < n; i++)
        sum += x[i]*y[i];
    return sum;
} /* Serial_dot */