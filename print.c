#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void Print_row(int local_mat[], int n, int my_rank, int i);

/*---------------------------------------------------------------------
 * Function:  Print_row
 * Purpose:   Convert a row of local_mat to a string and then print
 *            the row.  This tends to reduce corruption of output
 *            when multiple processes are printing.
 * In args:   all            
 */
void Print_row(int local_mat[], int n, int my_rank, int i){
   char char_int[100];
   char char_row[1000];
   int j, offset = 0;

   for (j = 0; j < n; j++) {
      if (local_mat[i*n + j] == INFINITY)
         sprintf(char_int, "i ");
      else
         sprintf(char_int, "%d ", local_mat[i*n + j]);
      sprintf(char_row + offset, "%s", char_int);
      offset += strlen(char_int);
   }  
   printf("Proc %d > row %d = %s\n", my_rank, i, char_row);
}  /* Print_row */