/*
Each thread and the main thread can carry 
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

Compile:   gcc -g -Wall -o pth stuff.c my_rand.c -lpthread
Run:       ./pth 3
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

/* Struct for the tasks */
struct task {
   char operation;
   int number;
};

struct list_node_s* head = NULL;
  
int thread_count;  //# of threads, and number of blocks to be used
int total_tasks; //# of tasks to be performed

pthread_rwlock_t rwlock;
pthread_mutex_t count_mutex;
pthread_cond_t condition;

int inserts = 0; //# of items in the list to start with (for delete)
int global = 0; //tells if the # of operations have been completed


void Usage(char* prog_name);
void Insert(int value);
void Delete(int value); 
void Member(int value);  
void Print(void);
void Get_input(int* inserts_p);
void* Hello(void* rank);

/*-----------------------------------------------------------------*/
int main(int argc, char* argv[]) {
   long i = 0; 
   int inserts;
   int number;
   pthread_t* thread_handles;
   unsigned seed = 1;
   char op;
   int num;
   int blocks;
   long thread;
   struct task* list;

   //make sure the proper amount of arguments are being used
   //then enter the number of threads to a variable from args
   if (argc != 2) {
      Usage(argv[0]);
   }
   thread_count = strtol(argv[1], NULL, 10);

   //figure out how many numbers to enter in the list 
   Get_input(&inserts);  

   while (i < inserts){
      number = my_rand(&seed) % MAX;
      Insert(number);
      i ++;
   }
   printf("This is the list before threads are started.\n");
   Print();

   printf("Starting thread creation.\n");
   thread_handles = malloc(thread_count * sizeof (pthread_t));

   printf("Hello from main thread\n");

   
   pthread_mutex_init(&count_mutex, NULL);
   pthread_rwlock_init(&rwlock, NULL);

   //make all threads wait
   pthread_cond_wait(&condition, &count_mutex);

   //check global var to see if it is a 0 if yes
   if (global == 0){
   //main thread asks for a # of how many ops to execute
      printf("How many blocks of tasks would you like?\n"); 
      scanf("%d", &blocks);
      printf("How many tasks per block?\n");
      scanf("%d", &total_tasks);
      for(int j = 0; j < blocks; j ++){
         list = (struct task*) 
            malloc(total_tasks * sizeof(struct task));
         for (int k = 0; k < total_tasks; k ++){
            printf("Please enter an operation, and a number\n");
            scanf("%c %d", &op, &num);
            (list + j) -> operation = op;
            (list + j) -> number = num;
         }
         //wake up a thread and perform the ops in this block with
         //a condition signal
         //go through that block and perform ops
         //make thread go back to waiting with condition

         count ++;
      }
      if(count == blocks){
         global = 1;
      }
    }
    else{
      //main thread needs to send condition broadcast signal
      //print out the modified list
      //and print out which ops were carried out by which threads
    }





   return 0;
}  /* main */


/*-----------------------------------------------------------------*/
void Usage(char* prog_name) {
   fprintf(stderr, "Usage: %s <thread_count>\n", prog_name);
   exit(0);
}  /* Usage */


/*-----------------------------------------------------------------*/
/* Insert value in correct numerical location into list */
void Insert(int value) {
   struct list_node_s* curr = head;
   struct list_node_s* pred = NULL;
   struct list_node_s* temp;
   
   while (curr != NULL && curr -> data < value) {
      pred = curr;
      curr = curr -> next;
   }

   if (curr == NULL || curr -> data > value) {
      temp = malloc(sizeof(struct list_node_s));
      temp -> data = value;
      temp -> next = curr;
      if (pred == NULL)
         head = temp;
      else
         pred -> next = temp;
   } else { /* value in list */
      printf("This value is already in the list.\n");
   }
}  /* Insert */



/*-----------------------------------------------------------------*/
/* Deletes value from list */
void Delete(int value) {
   struct list_node_s* curr = head;
   struct list_node_s* pred = NULL;

   while (curr != NULL && curr -> data < value) {
      pred = curr;
      curr = curr -> next;
   }
   
   if (curr != NULL && curr -> data == value) {
      if (pred == NULL) { /* first element in list */
         head = curr -> next;
         free(curr);
      } else { 
         pred -> next = curr -> next;
         free(curr);
      }
   } else { /* Not in list */
      printf("This value was not in the list.\n");
   }
}  /* Delete */


/*-----------------------------------------------------------------*/
/* Finds the value in the list */
void  Member(int value) {
   struct list_node_s* temp;

   temp = head;
   while (temp != NULL && temp -> data < value)
      temp = temp -> next;

   if (temp == NULL || temp -> data > value) {
      printf("This item is not in the list.\n");
   } else {
      printf("This item is in the list.\n");
   }
}  /* Member */



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

   printf("How many numbers should we have?\n");
   scanf("%d", inserts_p);
}  /* Get_input */

























