#include <stdio.h>
#include <stdlib.h>

int scratch[100];

void Read_list(int list[], int n);
void Print_list(int list[], int n);
void Mergesort(int list[], int n);
void Merge(int list1[], int list2[], int n);

int main(int argc, char* argv[]) {
   int n;
   int list[100];

   printf("How many elements in the list?\n");
   scanf("%d", &n);
   printf("Enter the elements of the list\n");
   Read_list(list, n);

   /* n is a power of 2 */
   Mergesort(list, n);

   printf("The sorted list is\n");
   Print_list(list, n);

   return 0;
}

void Read_list(int list[], int n) {
   int i;

   for (i = 0; i < n; i++)
      scanf("%d", &list[i]);
}  /* Read_list */

void Print_list(int list[], int n) {
   int i;

   for (i = 0; i < n; i++)
      printf("%d ", list[i]);
   printf("\n");
}  /* Print_list */


/* You'll pass in a pointer to the head and the tail of the list */
void Mergesort(int list[], int n) {
   int list_sz = 2, curr_sz;

   printf("Sorting list:\n");
   Print_list(list, n);

   if (n == 1) {
      printf("Sorted list:\n");
      Print_list(list, n);
      return;
   }

   for (list_sz = 2; list_sz <= n; list_sz *= 2) {
      curr_sz = list_sz/2;

      /* The head of this list is just after the tail of the first half */
      Mergesort(list + curr_sz, curr_sz);

      printf("Merging\n");
      Print_list(list, curr_sz);
      Print_list(list + curr_sz, curr_sz);

      
      Merge(list, list + curr_sz, curr_sz);

      printf("Merged list:\n");
      Print_list(list, n);
      printf("\n");
   }

   printf("Sorted list:\n");
   Print_list(list, n);
}  /* Mergesort */


void Merge(int list1[], int list2[], int n) {
   int i1, i2, is;

   i1 = i2 = is = 0;
   while (i1 < n && i2 < n) {
      if (list1[i1] <= list2[i2])
         scratch[is++] = list1[i1++];
      else
         scratch[is++] = list2[i2++];
   }
   
   for (; i1 < n; i1++)
      scratch[is++] = list1[i1];
   for (; i2 < n; i2++)
      scratch[is++] = list2[i2];

   for (i1 = 0, is = 0; i1 < 2*n; i1++, is++)
      list1[i1] = scratch[is];
}  /* Merge */
