#include <stdio.h>
#include <string.h>
#include <stdlib.h>
char **string_split(const char *input, const char *sep, int *num_words){
    //array of words
    char**result=NULL;
    //dupe of input
    char *stri = strdup(input);

    //length of characters efore sep
    int stlength=0;

    //amount to remove from stri
    int size = 0;

    //`
    int word = 0;

    //if either are empty strings return
    if(strlen(sep)==0 || strlen(input)==0){
        exit(0);
    }

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
      
      //i is the amount that must be cut off of stri
      size+= strspn(stri,sep);
      size+= stlength;

      //update stri to remove what we just processed
      stri = &stri[size];

      //get rid of the newline if last word
      if(strlen(stri)==0){
         result[word] = realloc(result[word], stlength);
         //make the word "\0" terminated
         result[word][stlength-1]= '\0';
      }else{
         result[word][stlength]= '\0';
      }

      //we just got one word
      word++;
      //reset size
      size=0;
      //fix word count if middle!!
      if((stlength==0 && word !=1)&&(strlen(stri)!=0)){
         word--;
      }
      //realloc for the next word or DONT if it is an empty one in the middle
      if(strlen(stri)!=0 && (stlength!=0 || word ==1)){
          result = realloc(result,sizeof(char*)*word+1);
        }
      }
      *num_words = word;
      stri =NULL;
      free(stri);
        return result;
 }

 int main(int argc, char* argv[]){
   //somewhere in here before calling, make sep using strdup(source)
   //makes sep
   char* source = NULL;
   char**result=NULL;

   if(argc != 1){
      //variable for size of sep. Should start at the size of the first argument as long as there is an argument
      int j =strlen(argv[1]);
      source = malloc(j);
      //realloc that
      for(int i = 1; i < argc; i++){
         strcat(source,argv[i]);
         if(i<argc-1){
         j+=sizeof(argv[i+1]);
         }
         source = realloc(source,j);
      }
   }else{
      source = malloc(1);
      source[0]='\t';
   }
   int nub = 0;
   int* numb = &nub;

   //buffer to store all of the lines of input
   char imp[4000];

   //read in all lines until "."
   while(1==1){
      fgets(imp,4000,stdin);
      //i want to break FREEEEE
      if(result!=NULL){
         for(int i=0;i<nub;i++){
            free(result[i]);
         }
      }
      //special exit
      if(strcmp(imp,".\n")==0){
         free(result);
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


 
