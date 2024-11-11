#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "split.h"

int main(int argc, char* argv[]){
   //somewhere in here before calling, make sep using strdup(source)
   //makes sep
   char* source = malloc(1);
   char**result=NULL;

   if(argc > 1){
      //variable for size of sep. Should start at the size of the first argument as long as there is an argument
      source[0] = '\0';
      int j =strlen(argv[1]) +1;
      source = realloc(source,j);
      for(int i = 1; i < argc; i++){ 
         strcat(source,argv[i]);
         if(i<argc-1){
            j += strlen(argv[i+1]);
         }//realloc for next one
         source = realloc(source,j);
      }

   }else{
      source = realloc(source,3);
      source[0]='\t';
      source[1]= ' ';
      source[2]='\0';
   }

   int nub = 0;
   int* numb = &nub;

   //buffer to store all of the lines of input
   char imp[4000];

   //read in all lines until "."
   while(1==1){
      fgets(imp,4000,stdin);
      //remove /n
      imp[strcspn(imp, "\n")] = 0;
      //i want to break FREEEEE
      if(result!=NULL){
         for(int i=0;i<nub;i++){
            free(result[i]);
         }
         free(result);
      }
      //special exit
      if(strcmp(imp,".")==0){
         free(source);
         numb=NULL;
         free(numb);
         exit(0);
      }
      result = string_split(imp,source,numb);
      for(int i =0;i< nub; i++){
         printf("[%s]",result[i]);
         }
         printf("\n");
      }
 }