#include <stdio.h>
#include <string.h>
#include "split.h"
#include <stdlib.h>

char **string_split(const char *input, const char *sep, int *num_words){
    char* pointer = strdup(input);
    //array of words
    char**result=NULL;
    //dupe of input
    char *stri = pointer; 

    //length of characters efore sep
    int stlength=0;

    //amount to remove from stri
    int size = 0;

    //words for num
    int word = 0;

    //since there will be at least one item in result, it will start at 1 word
    result = calloc(sizeof(char *), 1);

    //checks to see if there is still anything in the string
    while(strlen(stri)!=0){
      //length of first word without sep
      stlength = strcspn(stri,sep);
      //make room for this word in the result array PLUS the null character
      result[word] = calloc(sizeof(char),stlength+1);
      //place the word in the result array
      strncpy(result[word],stri,stlength);
      
      //size is the amount that must be cut off of stri
      size+= strspn(stri,sep);
      size+= stlength;

      //update stri to remove what we just processed
      stri = &stri[size];

      //we just got one word
      word++;
      //reset size
      size=0;
      //fix word count if middle!!
      if((stlength==0 && word !=1)&&(strlen(stri)!=0)){
         word--;
         free(result[word]);
      }
      //realloc for the next word or DONT if it is an empty one in the middle
      if(strlen(stri)!=0 && (stlength!=0 || word ==1)){
          result = realloc(result,sizeof(char*)*(word+1));
        }
      }
      *num_words = word;

      free(pointer);
      return result;
 }