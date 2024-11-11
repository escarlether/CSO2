#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include<string.h>
#include <sys/wait.h>

int my_system(const char *command){
    pid_t child;
    int status;
    //char* sys =  malloc(strlen(command));
    //strcpy(sys,command);
    //char* original_token = strtok(sys,";");
    //char* token = original_token;
    
  
        child = fork();
        if(child>0){
            waitpid(child,&status,0);
        }
        if(child==0){
            execl("/bin/sh", "sh", "-c",command,NULL);
            return child;
        }
        //token = strtok(NULL, ";");

    return status;
}