/* File:    serial_mat_vect.c 
 *
 * Purpose: Computes a matrix-vector product on a single processor.
 *
 * Compile: gcc -g -Wall -o serial_mat_vect serial_mat_vect.c
 * Run:     ./serial_mat_vect
 *
 * Input:   m, n: order of matrix
 *          A, x:  the matrix and the vector to be multiplied
 *
 * Output:  y: the product vector
 *
 * Note:    A is stored as a 1-dimensional array.
 */
#include <stdio.h>
#include <stdlib.h>

void Read_matrix(char* prompt, float A[], int m, int n);
void Read_vector(char* prompt, float v[], int n);
void Serial_matrix_vector_prod(float A[], int m, int n,
          float x[], float y[]);
void Print_matrix(char* title, float A[], int m, int n);
void Print_vector(char* title, float y[], int n);
    
int main(void) {
    float*    A; 
    float*    x;
    float*    y;
    int       m, n;

    printf("Enter the order of the matrix (m x n)\n");
    scanf("%d %d", &m, &n);
    A = malloc(m*n*sizeof(float));
    Read_matrix("the matrix", A, m, n);
    x = malloc(n*sizeof(float));
    Read_vector("the vector", x, n);

    Print_matrix("We read", A, m, n);
    Print_vector("We read", x, n);
    y = malloc(m*sizeof(float));

    Serial_matrix_vector_prod(A, m, n, x, y);
    Print_vector("Result is", y, m);

    free(A);
    free(x);
    free(y);
    return 0;
}  /* main */
   

/*---------------------------------------------------------------
 * Function:  Read_matrix
 * Purpose:   Read a matrix from stdin
 * In args:   prompt:  tell user what to enter
 *            m:  number of rows
 *            n:  number of cols
 * Out arg:   A:  the matrix
 */
void Read_matrix(
         char*     prompt  /* in  */,
         float     A[]     /* out */,
         int       m       /* in  */,
         int       n       /* in  */) {
    int i, j;

    printf("Enter %s\n", prompt);
    for (i = 0; i < m; i++)
        for (j = 0; j < n; j++)
            scanf("%f", &A[i*n + j]);
}  /* Read_matrix */


/*---------------------------------------------------------------
 * Function: Read_vector
 * Purpose:  Read a vector from stdin
 * In args:  prompt:  tell user what to enter
 *           n:  number of components in vector
 * Out arg:  v:  vector
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
 * Function:  Serial_matrix_vector_prod
 * Purpose:   Find a matrix-vector product
 * In args:   A:  the matrix
 *            m:  the number of rows in A (and entries in y)
 *            n:  the number of cols in A (and entries in x)
 *            x:  the vector being multiplied by A
 * Out args:  y:  the product vector Ax
 */
void Serial_matrix_vector_prod(
        float     A[]  /* in  */,
        int       m    /* in  */,
        int       n    /* in  */,
        float     x[]  /* in  */,
        float     y[]  /* out */) {

    int i, j;

    for (i = 0; i < m; i++) {
        y[i] = 0.0;
        for (j = 0; j < n; j++)
            y[i] += A[i*n + j]*x[j];
    }
}  /* Serial_matrix_vector_prod */


/*---------------------------------------------------------------
 * Function:  Print_matrix
 * Purpose:   Print a matrix
 * In args:   title:  what we're printing
 *            A:  the matrix
 *            m:  the number of rows
 *            n:  the number of cols
 */
void Print_matrix(
         char*     title   /* in  */,
         float     A[]     /* out */,
         int       m       /* in  */,
         int       n       /* in  */) {
    int i, j;

    printf("%s = \n", title);
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++)
            printf("%4.1f ", A[i*n + j]);
        printf("\n");
    }
}  /* Print_matrix */


/*---------------------------------------------------------------
 * Function:  Print_vector
 * Purpose:   Print a vector
 * In args:   title:  what we're printing
 *            y:  the vector
 *            n:  the number of components in y
 */
void Print_vector(
         char*  title  /* in */,
         float  y[]    /* in */,
         int    n      /* in */) {
    int i;

    printf("%s\n", title);
    for (i = 0; i < n; i++)
        printf("%4.1f ", y[i]);
    printf("\n");
}  /* Print_vector */
