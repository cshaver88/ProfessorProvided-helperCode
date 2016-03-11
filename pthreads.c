/*
For the Pthreads program you should first write a shared 
linked list program:  just make it a sorted singly-linked 
list of ints.  Each thread and the main thread can carry 
out an insert, delete, or member.  Use Pthreads read-write 
locks to ensure that any number of threads can execute 
member simultaneously, but if a thread needs to insert 
or delete, it will obtain exclusive access to the list.

For the second part of the program you should build a 
"task queue".  The main thread begins by starting a 
user-specified number of threads that immediately go t
o sleep in a condition wait. The main thread reads in blocks 
of tasks to be carried out by the other threads; each time 
it generates a new block of tasks, it awakens a thread with 
a condition signal. When a thread finishes executing its 
block of tasks, it should return to a condition wait. 
When the main thread completes generating tasks, it sets 
a global variable indicating that there will be no more 
tasks, and awakens all the threads with a condition broadcast.

Input will be a list of blocks of tasks.  The tasks are 
linked list operations, which can be represented as a pair.   
The first element of the pair is a char representing the 
operation:  'i' for insert, 'd' for delete, and 'm' for 
member.  The second element is a value to be inserted, 
deleted, or searched for.  A block of tasks is also a pair.  
It consists of an array of tasks and an int specifying the 
number of tasks.  

The main thread should read one block of tasks at a time 
and store it in memory.  Then it should execute a condition 
signal, and the task it awakens should carry out the 
specified operations.  

When there is no more input, the main thread should print 
the linked list, and which operations were carried out by 
which threads.
*/


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "my_rand.h"

/* Random ints are less than MAX_KEY */
const int MAX = 100000000;


/* Struct for list nodes */
struct list_node_s {
   int data;
   struct list_node_s* next;
};


struct list_node_s* head = NULL;
  
int thread_count;
int total_ops;

pthread_rwlock_t rwlock;
pthread_mutex_t count_mutex;

int inserts = 0;

double insert_percent;
double search_percent;
double delete_percent;

int member_count = 0, insert_count = 0, delete_count = 0;


void Usage(char* prog_name);
void Get_input(int* inserts_p);
int Insert(int number);
void Print(void);
void Free_List(void);
void* Thread_Work(void* rank);
int Delete(int value);
int Member(int value);
int Is_empty(void);


/*-----------------------------------------------------------------*/
int main(int argc, char* argv[]) {
   long i = 0; 
   int inserts;
   int number;
   pthread_t* thread_handles;
   unsigned seed = 1;

   // int key;
   // int success;
   // int attempts;
   // int inserts_in_main;
   // double start, finish;

   //make sure the proper amount of arguments are being used
   //then enter the number of threads to a variable from args
   if (argc != 2) {
      Usage(argv[0]);
      thread_count = strtol(argv[1], NULL, 10);
   }

   //figure out how many numbers to enter in the list
   Get_input(&inserts);  

   while (i < inserts){
      // number = my_rand(&seed) % MAX_KEY;
      number = my_rand(&seed) % MAX;
      Insert(number);
      i ++;
   }
   printf("This is the list before threads are started.\n");
   Print();

   printf("Starting thread creation.\n");
   thread_handles = malloc(thread_count * sizeof (pthread_t));
   pthread_mutex_init(&count_mutex, NULL);
   pthread_rwlock_init(&rwlock, NULL);

   for (int j = 0; j < thread_count; j ++){
      pthread_create(&thread_handles[i], NULL, Thread_Work, (void*) i);
   }

   for(int k = 0; k < thread_count; k ++){
      pthread_join(thread_handles[k], NULL);
   }

   printf("This is the modified list after threads are finished\n");
   Print();

   printf("Freeing the list of all of its items\n");
   Free_List();

   printf("Destroying the lock and mutex\n");
   pthread_rwlock_destroy(&rwlock);
   pthread_mutex_destroy(&count_mutex);
   free(thread_handles);


   return 0;
}  /* main */


/*-----------------------------------------------------------------*/
void Usage(char* prog_name) {
   fprintf(stderr, "Usage: %s <thread_count>\n", prog_name);
   exit(0);
}  /* Usage */


/*-----------------------------------------------------------------*/
/* Insert value in correct numerical location into list */
/* If value is not in list, return 1, else return 0 */
int Insert(int value) {
   struct list_node_s* curr = head;
   struct list_node_s* pred = NULL;
   struct list_node_s* temp;
   int rv = 1;
   
   while (curr != NULL && curr->data < value) {
      pred = curr;
      curr = curr->next;
   }

   if (curr == NULL || curr->data > value) {
      temp = malloc(sizeof(struct list_node_s));
      temp->data = value;
      temp->next = curr;
      if (pred == NULL)
         head = temp;
      else
         pred->next = temp;
   } else { /* value in list */
      rv = 0;
   }

   return rv; /* value is not in list */
}  /* Insert */


/*-----------------------------------------------------------------*/
void Print(void) {
   struct list_node_s* temp;

   printf("list = ");

   temp = head;
   while (temp != (struct list_node_s*) NULL) {
      printf("%d ", temp->data);
      temp = temp->next;
   }
   printf("\n");
}  /* Print */



/*-----------------------------------------------------------------*/
void Get_input(int* inserts_p) {

   printf("How many keys should be inserted in the main thread?\n");
   scanf("%d", inserts_p);
}  /* Get_input */


/*-----------------------------------------------------------------*/
void Free_List(void) {
   struct list_node_s* current;
   struct list_node_s* following;

   if (Is_empty()) return;
   current = head; 
   following = current -> next;
   while (following != NULL) {
#     ifdef DEBUG
      printf("Freeing %d\n", current -> data);
#     endif
      free(current);
      current = following;
      following = current -> next;
   }
#  ifdef DEBUG
   printf("Freeing %d\n", current -> data);
#  endif
   free(current);
}  /* Free_list */


/*-----------------------------------------------------------------*/
void* Thread_Work(void* rank) {
   long my_rank = (long) rank;
   int i, val;
   double which_op;
   unsigned seed = my_rank + 1;
   int my_member_count = 0, my_insert_count = 0, my_delete_count = 0;
   int ops_per_thread = total_ops/thread_count;

   for (i = 0; i < ops_per_thread; i++) {
      which_op = my_drand(&seed);
      val = my_rand(&seed) % MAX;
      if (which_op < search_percent) {
         pthread_rwlock_rdlock(&rwlock);
         Member(val);
         pthread_rwlock_unlock(&rwlock);
         my_member_count++;
      } else if (which_op < search_percent + insert_percent) {
         pthread_rwlock_wrlock(&rwlock);
         Insert(val);
         pthread_rwlock_unlock(&rwlock);
         my_insert_count++;
      } else { /* delete */
         pthread_rwlock_wrlock(&rwlock);
         Delete(val);
         pthread_rwlock_unlock(&rwlock);
         my_delete_count++;
      }
   }  /* for */

   pthread_mutex_lock(&count_mutex);
   member_count += my_member_count;
   insert_count += my_insert_count;
   delete_count += my_delete_count;
   pthread_mutex_unlock(&count_mutex);

   return NULL;
}  /* Thread_work */


/*-----------------------------------------------------------------*/
/* Deletes value from list */
/* If value is in list, return 1, else return 0 */
int Delete(int value) {
   struct list_node_s* curr = head;
   struct list_node_s* pred = NULL;
   int rv = 1;

   /* Find value */
   while (curr != NULL && curr->data < value) {
      pred = curr;
      curr = curr->next;
   }
   
   if (curr != NULL && curr->data == value) {
      if (pred == NULL) { /* first element in list */
         head = curr->next;
#        ifdef DEBUG
         printf("Freeing %d\n", value);
#        endif
         free(curr);
      } else { 
         pred->next = curr->next;
#        ifdef DEBUG
         printf("Freeing %d\n", value);
#        endif
         free(curr);
      }
   } else { /* Not in list */
      rv = 0;
   }

   return rv;
}  /* Delete */


/*-----------------------------------------------------------------*/
int  Member(int value) {
   struct list_node_s* temp;

   temp = head;
   while (temp != NULL && temp->data < value)
      temp = temp->next;

   if (temp == NULL || temp->data > value) {
#     ifdef DEBUG
      printf("%d is not in the list\n", value);
#     endif
      return 0;
   } else {
#     ifdef DEBUG
      printf("%d is in the list\n", value);
#     endif
      return 1;
   }
}  /* Member */


/*-----------------------------------------------------------------*/
int  Is_empty(void) {
   if (head == NULL)
      return 1;
   else
      return 0;
}  /* Is_empty */








