/* File:     search_word1.c
 *
 * Purpose:  Search input text for every occurrence of a word.  Print
 *           where the word occurs, and the total number of occurrences.
 *           This version gets the word from the command line and
 *           the input text from stdin.
 *
 * Compile:  gcc -g -Wall -o search_word search_word.c
 * Usage:    ./search_word1 <word to be searched for>
 *
 * Input:    text
 * Output:   Location of each occurrence of the search word.
 *           Total number of occurrences of the search word.
 *              
 * Notes:
 * 1.  Word searched for should contain no white space
 * 2.  Words in input text consist of strings separated by white space
 * 3.  No error check for strings exceeding MAX_STRING_LEN
 * 4.  If the input text comes from stdin, the locations of
 *     the search word will be printed after each line of input.
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>  /* For exit */

const int MAX_STRING_LEN = 100;

void Usage(char prog_name[]);

int main(int argc, char* argv[]) {
   char search_for[MAX_STRING_LEN];
   char current_word[MAX_STRING_LEN];
   int  scanf_rv;
   int  total_word_count = 0;  
   int  search_word_count = 0;

   /* Check that the user-specified word is on the command line */
   if (argc != 2) Usage(argv[0]);
   strcpy(search_for, argv[1]);

   printf("Enter the text to be searched\n");
   scanf_rv = scanf("%s", current_word);
   while ( scanf_rv != EOF ) {
      if ( strcmp(current_word, search_for) == 0 ) {
         printf("%s is word %d\n", search_for, total_word_count);
         search_word_count++;
      }
      total_word_count++; 
      scanf_rv = scanf("%s", current_word);
   }

   printf("%s occurred %d times in the input\n", 
         search_for, search_word_count);

   return 0;
}  /* main */

/* If user-specified word isn't on the command line, 
 * print a message and quit
 */
void Usage(char prog_name[]) {
   fprintf(stderr, "usage: %s <string to search for>\n",
      prog_name);
   exit(0);
}  /* Usage */