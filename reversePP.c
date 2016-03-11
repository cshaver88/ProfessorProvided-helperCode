/* File:     reverse.c
 * Purpose:  Reverse the order of the elements in a user input array
 *
 * Compile:  gcc -g -Wall -o reverse reverse.c
 * Run:      ./reverse
 *
 * Input:    The number of elements in the array (<= 20) and the elements
 *           of the array.
 * Output:   The input array and the array with the elements reversed.
 *
 * Note:     The maximum number of elements in the array is 20.
 */
#include <stdio.h>

const int MAX_ARR = 20;

void Read_arr(int arr[], int n);
void Reverse_arr(int arr[], int n);
void Print_arr(int arr[], int n);

int main(void) {
   int arr[MAX_ARR];
   int n;

   printf("How many elements in the array?\n");
   scanf("%d", &n);
   printf("Enter the elements of the array\n");
   Read_arr(arr, n);
   printf("The original array is\n");
   Print_arr(arr, n);

   Reverse_arr(arr, n);

   printf("The array with the elements reversed is\n");
   Print_arr(arr, n);

   return 0;
}  /* main */

/*---------------------------------------------------------------------
 * Function:  Read_arr
 * Purpose:   Read an array of ints from stdin
 * Out arg:   arr, the array being input
 * In arg:    n, the number of elements in the array
 */
void Read_arr(int arr[], int n) {
   int i;

   for (i = 0; i < n; i++)
      scanf("%d", &arr[i]);
}  /* Read_arr */


/*---------------------------------------------------------------------
 * Function:   Reverse_arr
 * Purpose:    Reverse the elements in an array
 * In/out arg: arr, the array being reversed
 * In arg:     n, the number of elements in the array
 */
void Reverse_arr(int arr[], int n) {
   int i, tmp;

   for (i = 0; i < n/2; i++) {
      tmp = arr[i];
      arr[i] = arr[n-i-1];
      arr[n-i-1] = tmp;
   }
}  /* Reverse_arr */


/*---------------------------------------------------------------------
 * Function:  Print_arr
 * Purpose:   Read an array of ints to stdout
 * In args:   arr, the array being printed
 *            n, the number of elements in the array
 */
void Print_arr(int arr[], int n) {
   int i;

   for (i = 0; i < n; i++)
      printf("%d ", arr[i]);
   printf("\n");
}  /* Print_arr */