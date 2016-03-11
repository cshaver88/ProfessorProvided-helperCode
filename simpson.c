/* File:    simpson.c
 * Purpose: Calculate area using trapezoidal rule and Simpson's rule and
 *          compare the results.
 *
 * Input:   a, b, n
 * Output:  estimate of area between x-axis, x = a, x = b, and graph of f(x)
 *          using n subintervals with the trapezoidal rule and Simpson's
 *          rule.
 *
 * Compile: gcc -g -Wall -o simpson simpson.c -lm
 * Usage:   ./simpson
 *
 * Notes:    
 * 1.  The function f(x) is hardwired.
 * 2.  The number of subintervals, n, should be even
 */

#include <stdio.h>
#include <math.h>

double Trap(double  a, double  b, int n, double h);
double Simpson(double  a, double  b, int n, double h);
double f(double x);    /* Function we're integrating */

int main(void) {
   double  area_t, area_s;  /* Store result in area       */
   double  a, b;            /* Left and right endpoints   */
   int     n;               /* Number of subintervals     */
   double  h;               /* Length of subintervals     */

   printf("Enter a, b, and n\n");
   scanf("%lf", &a);
   scanf("%lf", &b);
   scanf("%d", &n);

   h = (b-a)/n;
   area_t = Trap(a, b, n, h);
   area_s = Simpson(a, b, n, h);

   printf("With n = %d subintervals, our estimates\n", n);
   printf("of the area from %f to %f are\n", a, b);
   printf("    %.15f with the trapezoidal rule\n", area_t);
   printf("    %.15f with Simpson's rule\n\n", area_s);
   printf("The difference between the two is %e\n", 
         fabs(area_s-area_t));

   return 0;
}  /* main */

/*------------------------------------------------------------------
 * Function:     Trap
 * Purpose:      Estimate area using the trapezoidal rule
 * Input args:   a: left endpoint
 *               b: right endpoint
 *               n: number of trapezoids
 *               h: stepsize = length of base of trapezoids
 * Return val:   Trapezoidal rule estimate of area between x-axis,
 *               x = a, x = b, and graph of f(x)
 */
double Trap(double a, double b, int n, double h) {
    double area;   /* Store result in area  */
    double x;
    int i;

    area = (f(a) + f(b))/2.0;
    for (i = 1; i <= n-1; i++) {
        x = a + i*h;
        area = area + f(x);
    }
    area = area*h;

    return area;
} /*  Trap  */


/*------------------------------------------------------------------
 * Function:     Simpson
 * Purpose:      Estimate area using Simpson's rule
 * Input args:   a: left endpoint
 *               b: right endpoint
 *               n: number of subintervals
 *               h: stepsize = length of subintervals
 * Return val:   Simpson's rule estimate of area between x-axis,
 *               x = a, x = b, and graph of f(x)
 * Note:         Function assumes n is even.
 */
double Simpson(double  a, double  b, int n, double h) {
   double area, x, temp;
   int i;

   area = f(a) + f(b);

   temp = 0.0;
   for (i = 1; i <= n-1; i += 2) {
      x = a + i*h;
      temp += f(x);
   }
   area += 4*temp;
   
   temp = 0;
   for (i = 2; i <= n-2; i += 2) {
      x = a + i*h;
      temp += f(x);
   }
   area += 2*temp;

   area *= h;
   area /= 3;

   return area;
}  /* Simpson */


/*------------------------------------------------------------------
 * Function:    f
 * Purpose:     Compute value of function to be integrated
 * Input args:  x
 */
double f(double x) {
   double return_val;

   return_val = x*x + 1;
   return return_val;
}  /* f */