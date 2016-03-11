 
 // * Compile:  gcc -g -Wall -o ll linkedlist.c -lm
 // * Run:      ./ll
/*Write a program that implements merge sort on a linked list of strings.  
Input will be a list of strings.  You should build a linked list from the input list, 
print the input list, sort it using one of the two algorithms, 
and print the result and the time required for the sort.  The input list will come from stdin.  
The command line may include an "n" that will suppress output of the lists and just print the elapsed time for the sort.

The merge sort algorithm is a little different from the standard algorithm.  
Instead of recursively splitting the list in half,  you double the list size with each pass:

      for (list_size = 2; list_size <= n; list_size *= 2) {
            Sort(elements list_size/2, ..., list_size);
            Merge sublist 0,...,list_size/2-1 and sublist list_size/2, ... , list_size);
      }
You can initially assume that the list size is a power of two, 
but in order to get full credit, you need to correctly sort a list of any positive integer size.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "timer.h"

const int max = 100;

struct node {
   char  *string;
   struct node* next_p;
};

struct list_s {
   struct node* head;
   struct node* tail;
};
void Print2(struct list_s* list_p, int size);
struct list_s* Merge(struct list_s *list_p, struct list_s* new_list, int size);
struct list_s* Sort(struct list_s* list_p, int size) ;
struct node* RightSide(struct list_s* list_p, int size);
void Delete(struct node* node, struct list_s* list_p, int size);
void Insert(struct list_s* list_p, char string[]);
void GetString(char string[]);
void Print(struct list_s* list_p);
void Free(struct list_s* list_p);

/*-----------------------------------------------------------------*/
int main(void) {
	char string[max];
	struct list_s* list = malloc(sizeof(struct list_s));


	list -> head = list -> tail = NULL; //empty list

	double start = 0;
	double finish = 0;

	printf("Enter a size for the list\n");  //get the size of the list
	int size;
	scanf("%d", &size);

	for(int i = 0; i < size; i ++){
		GetString(string);
		Insert(list, string);
	}

	printf("This is the unsorted list.\n");
	Print(list);  //prints unsorted input list

	GET_TIME(start);  //starts the wall clock time

	list = Sort(list, size);  //sorts the input list 

	GET_TIME(finish);  //stops the wall clock time

	printf("This is the sorted list.\n");
	Print(list);  //prints the sorted list after merge

	Free(list);
	   
	printf("Elapsed time = %e seconds\n ", finish-start);

	return 0;
}  /* main */


//I'm attaching a short program that implements the merge sort using arrays. 
// The linked list version is basically the same, except that you don't
// need any scratch storage (except a couple of pointers):  
//when you merge, you can insert each new element into the new 
//list and delete it from the old list.
/*-----------------------------------------------------------------*/
/* Function:   Merge
 * Purpose:    Combine two halves of a sorted list
 */
struct list_s* Merge(struct list_s* left, struct list_s* right, int size) {
	struct node* current = left -> head;
	struct node* temp = right -> head;

	struct list_s* tempList = malloc(sizeof(struct list_s));
	int i ;
	int l = 0;
	int r = 0;

	for(i = 0; i < size ; i ++){
		if(l == size/2 && temp == NULL){
			return tempList;
		}
		else if(current == NULL || l == size/2){ //check to see if any of the lists are done
			Insert(tempList, temp -> string);
			temp = temp -> next_p;
			r++;
		}
		else if(temp == NULL || r == size/2){
			Insert(tempList, current -> string);
			current = current -> next_p;
			l++;
		}
		else if (strcmp(current -> string, temp -> string) == 0){
			//same string
			Insert(tempList, current -> string);
			Insert(tempList, temp -> string);
			current = current -> next_p;
			temp = temp -> next_p;
			l++;
			r++;
		}
		else if (strcmp(current -> string, temp -> string) < 0){
			//current is first
			Insert(tempList, current -> string);
			current = current -> next_p;
			l++;
		}
		else{
			//temp is first
			Insert(tempList, temp -> string);
			temp = temp -> next_p;
			r++;
		}
	}
	return tempList;

}  /* Merge */


/*------------------------------------------------------------------
 * Function:     Sort
 * Purpose:      Read the first 2 elems sort those, call Merge on 
 *				 4 elems while the 2nd and 3rd items are being sorted 
 * 				 recursively and so on...
 * Input args:   size = size of the array
 *				 list = original list
 * Return val:   void--none
 */
struct list_s* Sort(struct list_s* list_p, int size) {
	int list_size;
	int s = size / 2;
	int power = 1;
	for(int j = 0;j<s;j++){
		power *=2;
	}
	struct list_s* right = malloc(sizeof(struct list_s));
	struct list_s* left = malloc(sizeof(struct list_s));
	left->head = list_p->head;

	for(list_size = 2; list_size <= power; list_size *= 2){
		right -> head = RightSide(list_p, list_size);
		if(right->head ==NULL)
			return left;
		right = Sort(right, list_size/2);
		left = Merge(left, right, list_size);
	}
	return left;
} /*  Sort  */

/*------------------------------------------------------------------
 * Function:     RightSide
 * Purpose:      Get the right half of the array
 * Input args:   size = size of the array
 *				 list = list being sorted
 * Return:	 	 Pointer to the beginning of the right half of the list
 */
struct node* RightSide(struct list_s* list_p, int size) { 
	struct node* temp = list_p -> head;
	int right;
	for(right = 0; right < size / 2; right ++){
		temp = temp -> next_p;
	}
	return temp;
} /*  RightSide  */


/*-----------------------------------------------------------------*/
/* Function:   Insert
 * Purpose:    Insert new node in list
 * Input arg:  string = new string to be added to list
 */
void Insert(struct list_s* list_p, char string[]) {
   // struct node* current = list_p -> head;
   struct node* temp_p;

   temp_p = malloc(sizeof(struct node));
   temp_p -> string = malloc((strlen(string) + 1) * sizeof(char));
   strcpy(temp_p -> string, string);

	if (list_p -> head == NULL && list_p -> tail == NULL) {
        temp_p -> next_p = NULL;
		list_p -> head = list_p -> tail = temp_p;
	} else {
		list_p -> tail -> next_p = temp_p;
		list_p -> tail = temp_p;
	}

}  /* Insert */


/*-----------------------------------------------------------------*/
/* Function:   Get_string
 * Purpose:    Read the next string in stdin
 * Out arg:    string = next string in stdin
 */
void GetString(char string[]) {

   printf("Please enter a string: ");
   scanf("%s", string);
}  /* Get_string */


/*-----------------------------------------------------------------*/
/* Function:   Print
 * Purpose:    Print the contents of the nodes in the list
 * Input arg:  list_p = pointers to first and last nodes in list
 */
void Print(struct list_s* list_p) {
   struct node* current = list_p -> head;

   while (current != NULL) {
      printf("%s ", current -> string);
      current = current -> next_p;
   }
   printf("\n");
}  /* Print */

/*-----------------------------------------------------------------*/
/* Function:   Print
 * Purpose:    Print the contents of the nodes in the list
 * Input arg:  list_p = pointers to first and last nodes in list
 */
void Print2(struct list_s* list_p, int size) {
   struct node* current = list_p -> head;
   int i = 0;
   while (current != NULL && i<size) {
      printf("%s ", current -> string);
      current = current -> next_p;
      i++;
   }
   printf("\n");
}  /* Print */


/*-----------------------------------------------------------------*/
/* Function:   Free
 * Purpose:    Free up all storage that was used by the list and nodes
 */
void Free(struct list_s* list_p) {
   struct node* current;
   struct node* follow;

   current = list_p -> head; 
   while (current != NULL) {
      follow = current -> next_p;

      free(current -> string);
      free(current);
      current = follow;
   }

   list_p -> head = list_p -> tail = NULL;
}  /* Free */


