/* File:     input_data.c
 * Author:   Peter Pacheco
 *
 * Purpose:  Generate input for the heat equation program for which an
 *           exact solution is known.
 *
 * Compile:  gcc -g -Wall -o input_data input_data.c -lm
 * Run:      ./input_data
 *
 * Input:    m, the number of grid points in the x-direction is m+1
 *           n, the number of grid points in the t-direction is n+1
 *           k, a constant determining the "frequency" of the solution
 *           name of file in which the data should be stored
 * Output:   A file containing input data for program 1
 *
 * Note:
 *    With input k, the exact solution is
 *
 *       u(x,t) = exp(-k^2 pi^2 t) sin(k pi x)
 */

#include <stdio.h>
#include <math.h>

/* Maximum length for the file name */
const int MAX_NAME = 100;

int main(void) {
   int m, n, k, i;
   char filename[MAX_NAME];
   FILE* outfile;   // A file "pointer"
   double pi, x, h_x;

   printf("Enter m (m+1 = the number of grid points in the x-direction)\n");
   scanf("%d", &m);
   printf("Enter n (n+1 = the number of grid points in the t-direction)\n");
   scanf("%d", &n);
   printf("Enter k\n");
   scanf("%d", &k);

   printf("What's the name of the output file?\n");
   scanf("%s", filename);
   outfile = fopen(filename, "w");

   fprintf(outfile, "%d\n", m);
   fprintf(outfile, "%d\n", n);

   // Initial conditions
   pi = 4.0*atan(1.0);
   h_x = 1.0/m;
   for (i = 0; i <= m; i++) {
      x = i*h_x;
      fprintf(outfile, "%e ", sin(k*pi*x));
   }
   fprintf(outfile, "\n");

   fclose(outfile);
   return 0;
}  /* main */