#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include<string.h>
#include <sys/wait.h>


char *getoutput(const char *command){
    pid_t child;
    int status;
    int pipe_fd[2];
    pipe(pipe_fd);
    int read_fd = pipe_fd[0];
    int write_fd = pipe_fd[1];
    char* output = NULL;
    
    child = fork();
    char* individual = NULL;

    if(child>0){
        size_t len=0;
        size_t total =0;
        close(write_fd);
        FILE * fdfile = fdopen(read_fd,"r");
        while(1==1){
            if(getdelim(&individual,&len,'\0',fdfile)==-1){
                break; 
            }
            total+=len;
            if(output ==NULL){ 
                output = malloc(len);
                memcpy(output,individual,len);
            }else{
                output = realloc(output,total);
                memcpy(output,individual,len);
            }
        }
        close(read_fd);
        waitpid(child,&status,0);
    }
    else if(child==0){
        dup2(write_fd,STDOUT_FILENO);
        close(write_fd);
        close(read_fd);
        execl("/bin/sh", "sh", "-c",command,NULL);
    }
    free(individual);
    return output;
}

char *parallelgetoutput(int count, const char **argv_base){
    //make a pipe
    pid_t child[count];
    int status;
    int pipe_fd[2];
    pipe(pipe_fd);
    int read_fd = pipe_fd[0];
    int write_fd = pipe_fd[1];
    //reuturn values
    char* output = NULL;
    char* individual = NULL;
    int number=0;
            while(1==1){
                if(argv_base[number]==NULL){
                    break;
                }
                number++;
            }
    for(int i =0;i<count;i++){
        child[i]= fork();
        //child
        if(child[i]==0){
            dup2(write_fd,STDOUT_FILENO);
            close(write_fd);
            close(read_fd);
            char str[12];
            //malloc argv for sys call
            char ** argv = malloc((number+2)*sizeof(char*));
            //copy everything from argv_base into argv
            for(int j=0;j<number;j++){
                argv[j]= (char*)argv_base[j];
            }
            //make the second to last argument the process number
            sprintf(str,"%d",i);
            argv[number] = str;
            //make the last argument NULL
            argv[number+1] = NULL;
            //ececute
            execv(argv[0],argv);
            exit(0);
        }
    }
            size_t len=0;
            size_t total =0;
            close(write_fd);
            FILE * fdfile = fdopen(read_fd,"r");
            while(1==1){
                if(getdelim(&individual,&len,'\0',fdfile)==-1){
                    break; 
                }
                total+=len;
                if(output ==NULL){ 
                    output = malloc(len);
                    memcpy(output,individual,len);
                }else{
                    output = realloc(output,total);
                    memcpy(output,individual,len);
                }
            }
            free(individual);
            close(read_fd);

    //wait for all of the children
    for(int i =0;i< count;i++){
        waitpid(child[i],&status,0);
    }
    return output;

}
// int main() {
//     const char *argv_base[] = {
//         "/bin/echo", "running", NULL
//     };
//     const char *output = parallelgetoutput(2, argv_base);

//     printf("Text: [%s]\n", output);
//     free(output);
// }