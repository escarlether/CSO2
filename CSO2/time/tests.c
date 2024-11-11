#define _XOPEN_SOURCE 700
#include <stdlib.h>
#include <unistd.h>
#include <immintrin.h>
#include <sys/types.h>
#include <signal.h>
#include <time.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdbool.h>

long long start = 0;
long long end = 0;
bool recieved = false;

pid_t other_pid = 0;
pid_t myPID = 0;

__attribute__((noinline)) void empty_function(){
    __asm__("");
    return;
}

//i dont think that i am doing this correctly
//its supposed to measure the time it takes to run a progrom from the second the program 
//starts to the nanosecond it ends,maybe i will need to supply it with the starting time
long long nsecs() {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec*1000000000 + t.tv_nsec;
}

//will this be optimized away???
void handle_bus(int num){
    end = nsecs();
    recieved = true;
    return;
}

void handle_usr1(){
    end=nsecs();
    recieved =true;
}


void handle_usr2(){
    kill(other_pid,SIGUSR1);
}


int main(int argc, char* argv[]){
    FILE *fptr;
    fptr = fopen("timings.txt","a");
    long long tim=0;
    int counter = 0;
    long long over = 0;
    
    struct sigaction act;
    act.sa_handler = &handle_bus;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_RESTART;
    sigaction(SIGBUS,&act,NULL);
    
    act.sa_handler = &handle_usr1;
    sigaction(SIGUSR1,&act,NULL);

    act.sa_handler = &handle_usr2;
    sigaction(SIGUSR2,&act,NULL);


    pid_t myPID = getpid();
    //measure overhead
    for(int i=0;i<1000;i++){
        start =nsecs();
        //
        end = nsecs();
        over +=end-start;
    }
    over/=1000;

    switch(atoi(argv[1])){
        case 1: ;
            //warmup runs
            for(int i=0;i<1000;i++){
                empty_function();
            }
            start=0;
            end=0;
            while(tim < 500000000){
                start=nsecs();
                empty_function();
                end=nsecs();

                tim += end-start;
                counter++;
                tim-=over;
            }
            //get average
            tim/= counter;

            fprintf(fptr, "Average Case 1: %lld nanoseconds \n",tim);
            //30
            break;
        case 2: ;
            //warmup runs
            for(int i=0;i<10000;i++){
                getppid();
            }

            start=0;
            end=0;
            while(tim < 500000000){
                start=nsecs();
                getppid();
                end=nsecs();
                
                tim+= end - start;
                tim-=over;
                counter++;
            }
            //get average
            tim/=counter;
            //send to txt file
            fprintf(fptr, "Average Case 2: %lld nanoseconds \n",tim);
            //~600 not sure if this is correct
            break;
        case 3: ;
            //warmup runs
            for(int i=0;i<1000;i++){
                system("/bin/true");
            }

            start=0;
            end=0;
            while(tim < 500000000){
                start=nsecs();
                system("/bin/true");
                end=nsecs();
                
                tim+= end - start;
                tim-=over;
                counter++;
            }
            //get average
            tim/=counter;
            //send to txt file
            fprintf(fptr, "Average Case 3: %lld nanoseconds \n",tim);
            //~600 not sure if this is correct
            break;
        case 4: ;
            //warmup
            for(int i =0;i<100;i++){
                kill(myPID,SIGBUS);
                while(!recieved) { 
                    struct timespec ts = { .tv_sec = 0, .tv_nsec = 100 };
                    nanosleep(&ts, NULL);
                }
                recieved = false;
            }

            while(tim < 500000000){
                start=nsecs();

                kill(myPID,SIGBUS);
                while(!recieved) { 
                    struct timespec ts = { .tv_sec = 0, .tv_nsec = 10 };
                    nanosleep(&ts, NULL);
                }
                recieved = false;
                tim+= end - start;
                tim-=over;
                counter++;
            }
            tim/=counter;
            fprintf(fptr, "Average Case 4: %lld nanoseconds \n",tim);
            break;
        case 5: ;
            //get the pids
            printf("This process's ID: %ld\n", (long) getpid());
            char *line = NULL; size_t line_length = 0;
            printf("Enter other process ID: ");
            getline(&line, &line_length, stdin);
            other_pid = strtol(line, NULL, 10);
            free(line);
            
            //warmup
            for(int i =0;i<100;i++){
                kill(other_pid,SIGUSR2);
                while(!recieved) { 
                    struct timespec ts = { .tv_sec = 0, .tv_nsec = 100 };
                    nanosleep(&ts, NULL);
                }
                recieved= false;
            }


            while(tim < 500000000){
                start=nsecs();
                kill(other_pid,SIGUSR2);
                while(!recieved) { 
                    struct timespec ts = { .tv_sec = 0, .tv_nsec = 10 };
                    nanosleep(&ts, NULL);
                }
                recieved = false;

                tim+= end - start;
                tim-=over;
                counter++;
            }
            tim/=counter;
            fprintf(fptr, "Average Case 5: %lld nanoseconds \n",tim);
            kill(other_pid,SIGINT);

            break;
        case -1: ;
            //get the pids
            printf("This process's ID: %ld\n", (long) getpid());
            char *line2 = NULL; size_t line2_length = 0;
            printf("Enter other process ID: ");
            getline(&line2, &line2_length, stdin);
            other_pid = strtol(line2, NULL, 10);
            free(line2);

            while(1){
                struct timespec ts = { .tv_sec = 1234567890, .tv_nsec = 1000000000 };
                nanosleep(&ts, NULL);
            }
            
            break;
    }
    fclose(fptr);
}