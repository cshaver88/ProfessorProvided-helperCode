/* File:     fin_diff.c
 * Purpose:  Solve the one-dimensional heat equation on [0,1]x[0,1] using 
 *           finite differences.
 *
 * Compile:  gcc -g -Wall -o fin_diff fin_diff.c -lm
 * Run:      ./fin_diff
 *
 * Input:    m, the number of segments into which the bar is divided
 *           n, the number of time intervals
 *           u(x,0), the initial conditions           
 * Output:   u(x,t), for x = 0, 1/m, 2/m, . . . , (m-1)/m, 1 and 
 *           t = 0, 1/n, 2/n, 3/n, . . . , (n-1)/n, 1
 * 
 * Notes:
 * 1.  If the exact solution is known, add the code for the function
 *     u_exact and compile with -DEXACT. The difference between computed 
 *     u(x,1) and u_exact(x,1) will be printed at each time step.
 * 2.  DEBUG compile flag adds extra output.
 * 3.  Boundary conditions are 0:  u(0,t) = u(1,t) = 0, for all t
 */

#include <stdio.h>
#include <math.h>

const int MAX_X = 101;

void Get_input(double u[], int* m_p, int* n_p);
void Print_step(double t, double u[], int m);
void Copy_vals(double new_u[], double old_u[], int m);
void Print_exact(int m, double h_x, double t);
void Compare_exact(double u[], int m, double h_x, double t,
      double* max_err_p, double* max_err_x_p, double* max_err_t_p);
double u_exact(double x, double t);

/*-------------------------------------------------------------------*/
int main(void) {
   double new_u[MAX_X];
   double old_u[MAX_X];
   int m, n;
   double h_x, h_t, fact;
   double t;
   int int_x, int_time;
#  ifdef EXACT
   double max_err = 0.0;
   double max_err_x, max_err_t;
#  endif

   Get_input(new_u, &m, &n);
   h_x = 1.0/m;
   h_t = 1.0/n;
   fact = h_t/(h_x*h_x);
#  ifdef DEBUG
   printf("m = %d, n = %d\n", m, n);
   printf("h_x = %e, h_t = %e, fact = %e\n", h_x, h_t, fact);
#  endif
   Print_step(0, new_u, m);
#  ifdef EXACT
   Print_exact(m, h_x, 0.0);
   Compare_exact(new_u, m, h_x, 0.0, &max_err, &max_err_x, &max_err_t);
   printf("\n");
#  endif
   for (int_time = 1; int_time <= n; int_time++) {
      t = int_time*h_t;
      Copy_vals(new_u, old_u, m);
      new_u[0] = new_u[m] = 0.0;  // Boundary values are 0
      for (int_x = 1; int_x < m; int_x++)
         new_u[int_x] = old_u[int_x] + 
            fact*(old_u[int_x-1] - 2*old_u[int_x] + old_u[int_x+1]);
      Print_step(t, new_u, m);
#     ifdef EXACT
      Print_exact(m, h_x, t);
      Compare_exact(new_u, m, h_x, t, &max_err, &max_err_x, &max_err_t);
      printf("\n");
#     endif
   }

#  ifdef EXACT
   printf("max error = %e at (x, t) = (%e, %e)\n", 
         max_err, max_err_x, max_err_t);
#  endif

   return 0;
}  /* main */

/*-------------------------------------------------------------------*/
/* Function:     Get_input
 * Purpose:      Read the input data from stdin
 * Output args:  u:  the initial temperatures
 *               m_p:  pointer to the number of segments in the bar
 *               n_p:  pointer to the number of time intervals
 */
void Get_input(double u[], int* m_p, int* n_p){
   int i;

   printf("Enter m (m+1 = the number of grid points in the x-direction)\n");
   scanf("%d", m_p);
   printf("Enter n (n+1 = the number of grid points in the t-direction)\n");
   scanf("%d", n_p);
   printf("Enter the %d initial values of u\n", *m_p+1);
   for (i = 0; i <= *m_p; i++)
      scanf("%lf", &u[i]);
}  /* Get_input */

/*-------------------------------------------------------------------*/
/* Function:  Print_step
 * Purpose:   Print the time and the computed values for the current
 *            timestep
 * Input args: t:  current timestep
 *             u:  newly computed values of the temperature
 *             m:  the number of segments in the bar
 */
void Print_step(double t, double u[], int m) {
   int i;

   printf("%.3f ", t);
   for (i = 0; i <= m; i++)
      printf("%.3f ", u[i]);
   printf("\n");
}  /* Print_step */


/*-------------------------------------------------------------------*/
/* Function:    Copy_vals
 * Purpose:     Copy the newly computed u values into the old_u array
 * Input args:  new_u:  the newly computed u values
 *              m:  the number of segments in the bar
 * Output args: old_u:  stores a copy of the contents of new_u
 */
void Copy_vals(double new_u[], double old_u[], int m) {
   int i;

   for (i = 0; i <= m; i++)
      old_u[i] = new_u[i];
}  /* Copy_vals */

/*-------------------------------------------------------------------*/
/* Function:   Print_exact
 * Purpose:    Print the exact solution at timestep t
 * Input args: m:  the number of segments in the bar
 *             h_x:  the length of the segments
 *             t:  the current timestep
 */
void Print_exact(int m, double h_x, double t) {
   int i;
   double x;
   printf("%.3f ", t);
   for (i = 0; i <= m; i++) {
      x = i*h_x;
      printf("%.3f ", u_exact(x,t));
   }
   printf("\n");
}  /* Print_exact */

/*-------------------------------------------------------------------*/
/* Function:    Compare_exact
 * Purpose:     Compute absolute value of the difference between the
 *              program computed value and exact value at a timestep
 * Input args:  u:  compute values at current timestep
 *              m:  number of segments in bar (u stores m+1 values)
 *              h_x:  length of segments
 *              t:  current time
 * In/out args: max_err_p:  on input pointer to the maximum difference 
 *                 between the computed and exact solutions up to the
 *                 previous timestep.  On output updated to max difference
 *                 for all timesteps up to t.
 *              max_err_x_p:  Where the max difference occurs in the
 *                 x direction.                
 *              max_err_t_p:  Where the max difference occurs in the
 *                 t direction.
 * Note:  Only called if EXACT macro is defined
 */               
void Compare_exact(double u[], int m, double h_x, double t,
      double* max_err_p, double* max_err_x_p, double* max_err_t_p) {
   int i;
   double x, u_ex;
   double err;

   for (i = 0; i <= m; i++) {
      x = i*h_x;
      u_ex = u_exact(x,t);
      err = fabs(u_ex - u[i]);
      printf("%e ", err);
      if (err > *max_err_p) {
         *max_err_p = err;
         *max_err_x_p = x;
         *max_err_t_p = t;
      }
   }

   printf("\n");
}  /* Compare_exact */

/*-------------------------------------------------------------------*/
/* Function:    u_exact
 * Purpose:     Compute the exact value of the temperature at position x
 *              and time t
 * Input args:  x = position
 *              t= time
 * Return val:  u(x,t)
 * Notes:        
 * 1.  User needs to code this function.  For data generated
 *    by input_data.c, user only needs to define k.
 * 2.  Only called if EXACT macro is defined.
 */
double u_exact(double x, double t) {
   double pi = 4.0*atan(1.0);
   int k = 1;
   double t_fact;
   double x_fact;

   t_fact = exp(-k*k*pi*pi*t);
   x_fact = sin(k*pi*x);
   
   return t_fact*x_fact;
}  /* u_exact */