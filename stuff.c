/*
 
 Compile:   gcc -g -Wall -o pth stuff.c my_rand.c -lpthread
 Run:       ./pth 3
 
 
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
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "my_rand.h"


const int MAX = 100000000;  /* Random ints are less than MAX_KEY */
int thread_count;  //# of threads, and number of blocks to be used
int total_tasks; //# of tasks to be performed
pthread_rwlock_t rwlock;
pthread_mutex_t count_mutex;
pthread_cond_t condition;
int inserts = 0; //# of items in the list to start with (for delete)
int global = 0; //tells if the # of operations have been completed
struct list_node_s* head = NULL;



// ********************* GLOBAL THREAD CONDITIONAL VARIABLES ***************
int counter = 0;
pthread_mutex_t mutex;
pthread_cond_t cond_var;
// ********************* GLOBAL THREAD CONDITIONAL VARIABLES ***************



struct list_node_s                  /* Struct for list nodes */
{
   int data;
   struct list_node_s* next;
};

struct task                         /* Struct for the tasks */
{
   char operation;
   int number;
};

void Usage(char* prog_name);
void Insert(int value);
void Delete(int value); 
void Member(int value);  
void Print(void);
void Get_input(int* inserts_p);
void* Spawn_Threads(void* rank);



int main(int argc, char* argv[])
{
// *************************** LOCAL VARIABLES ******************************
    char op;
    unsigned seed = 1;
    long i = 0, thread;
    pthread_t* thread_handles;
    int inserts, number, num, blocks, block_count = 0;
// *************************** LOCAL VARIABLES ******************************

    
   //make sure the proper amount of arguments are being used
   //then enter the number of threads to a variable from args
   if (argc != 2) {
      Usage(argv[0]);
   }
    thread_count = strtol(argv[1], NULL, 10);

   //figure out how many numbers to enter in the list
   Get_input(&inserts);  

   while (i < inserts)
   {
      number = my_rand(&seed) % MAX;
      Insert(number);
      i ++;
   }
   printf("This is the list before threads are started.\n");
   Print();
    
    //  ***************** NEW function startThreads() *****************
    printf("Starting thread creation.\n");
    thread_handles = malloc(thread_count * sizeof(pthread_t));
    printf("Hello from the main thread\n");

    // create the user specified # of threads
    for (thread = 0; thread < thread_count; thread++)
        pthread_create(&thread_handles[thread], NULL, Spawn_Threads, (void*) thread);
    
    //immediately go to sleep
    // pthread_cond_wait(&condition, &mutex);

    //main thread asks for a # of how many blocks to execute
    printf("How many blocks of tasks would you like?\n");
    scanf("%d", &blocks);
    //then how many tasks should be in each block
    printf("How many tasks per block?\n");
    scanf("%d", &total_tasks);
    //and creates a list for those tasks
    struct task* list = (struct task*)
      malloc(total_tasks * sizeof(struct task));
    // if(global ==  0){
      // Just add this logic to Hello() for each thread to execute it.
      // Not sure how to have the threads spawn inside of main
      // They are spawned and call Hello() then terminate.
      for(int j = 0; j < blocks; j ++)
      {
          printf("Please enter an operation, and a number\n");
          scanf("%c %d", &op, &num);

          for (int k = 0; k < total_tasks; k ++)
          {
              //Spawn_Threads( (void*) thread );
              (list + j) -> operation = op;
              (list + j) -> number = num;
          }
          // pthread_cond_signal(&condition);
          //execute the block of tasks
          //add in switch case for list of tasks
          // ExecuteTasks(&list);
          // pthread_cond_wait(&condition, &mutex);
      }
      if (block_count == blocks){
        global = 1;
      }
    // }
    // else{
      //wake all threads with broadcast
      // pthread_cond_broadcast(&condition);
      // print the new list 
      //print out which thread did what
    // }
    // STOP all threads
    for (thread = 0; thread < thread_count; thread++)
        pthread_join(thread_handles[thread], NULL);
    // DESTROY all threads 
    free(thread_handles);

   return 0;
} /* Main */


/*-----------------------------------------------------------------*/
void ExecuteTasks(struct task* list) {
   for (int i = 0; i < list.size(); i ++){
      if ((list + i) -> operation == "i"){
        Insert((list + i) -> number);
      }
      else if ((list + i) -> operation == "d"){
        Delete((list + i) -> number);
      }
      else if ((list + i) -> operation == "m"){
        Member((list + i) -> number);
      }
      else{
        printf("This is an invalid operation.\n");
      }
   }
}  /* ExecuteTasks */


/*-----------------------------------------------------------------*/
void* Spawn_Threads(void* rank)
{
    long my_rank = (long) rank; // kludge for deriving thread #'s
    
// ********************* THREADS BARRIER *******************
    
    pthread_mutex_lock(&mutex);
    counter++;
    
    printf("Hello from thread %ld of %d. Counter == %d. \n", my_rank, thread_count, counter);

    if(counter == thread_count)
    {
        printf("All threads synced. Thread %ld setting counter. \n", my_rank);
        counter = 0;
        pthread_cond_broadcast(&cond_var);
    }
    else
    {
        printf("Thread %ld now waiting in barrier. \n", my_rank);
        while (pthread_cond_wait(&cond_var, &mutex) != 0);
    }
    
    pthread_mutex_unlock(&mutex);
// ********************* THREADS BARRIER *******************

    return NULL;
} /* Spawn_Threads */


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








