#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include<string.h>
#include <sys/wait.h>
#include "my_system.c"

int main(int argc, char* argv[]){

    int a1 = my_system("sleep 1; echo hi");
    int a2 = my_system("echo bye");
    int a3 = my_system("flibbertigibbet 23");
    printf("%d %d %d\n", 
        WEXITSTATUS(a1), WEXITSTATUS(a2), WEXITSTATUS(a3));

     my_system("echo -n 'type something: ';"
        " read got;"
        " echo Thanks for typing \\\"\"$got\"\\\"");
    return 0;
}