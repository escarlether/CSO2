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

long long nsecs() {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec*1000000000 + t.tv_nsec;
}

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
    //warmup overhead
    for(int i=0;i<10000000;i++){
        nsecs();
    }

    //measure nsecs to calculate overhead
    for(int i=0;i<900000;i++){
        start =nsecs();
        //
        end = nsecs();
        over +=end-start;
    }
    over/=900000;

    switch(atoi(argv[1])){
        case 1: ;
            //warmup runs
            for(int i=0;i<100000;i++){
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
                
            }
            //get raw average
            tim/= counter;
            //get average with overhead incorporated
            tim-=over;
            fprintf(fptr, "Average Case 1: %lld nanoseconds \n",tim);
            break;
        case 2: ;
            //warmup runs
            for(int i=0;i<1000000;i++){
                getppid();
            }

            start=0;
            end=0;
            while(tim < 500000000){
                start=nsecs();
                getppid();
                end=nsecs();
                
                tim+= end - start;
                counter++;
            }
            //get raw average
            tim/=counter;
            //get average with overhead incorporated
            tim-=over;
            //send to txt file
            fprintf(fptr, "Average Case 2: %lld nanoseconds \n",tim);
            break;
        case 3: ;
            //warmup runs
            for(int i=0;i<200;i++){
                system("/bin/true");
            }

            start=0;
            end=0;
            while(tim < 500000000){
                start=nsecs();
                system("/bin/true");
                end=nsecs();
                
                tim+= end - start;
                counter++;
            }
            //get raw average
            tim/=counter;
            //get average with overhead incorporated
            tim-=over;
            //send to txt file
            fprintf(fptr, "Average Case 3: %lld nanoseconds \n",tim);
            break;
        case 4: ;
            //warmup
            for(int i =0;i<10000;i++){
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
                counter++;
            }
            //get raw average
            tim/=counter;
            //get average with overhead incorporated
            tim-=over;
            fprintf(fptr, "Average Case 4: %lld nanoseconds \n",tim);
            break;
        case 5: ;
            //get the pids
            printf("This process's ID: %ld\n", (long) getpid());
            char *line = NULL; size_t line_length = 0;
            printf("Second: Enter other process ID: ");
            getline(&line, &line_length, stdin);
            other_pid = strtol(line, NULL, 10);
            free(line);
            //warmup
            for(int i =0;i<1000;i++){
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
                counter++;
            }
            //get raw average
            tim/=counter;
            //get average with overhead incorporated
            tim-=over;
            fprintf(fptr, "Average Case 5: %lld nanoseconds \n",tim);
            kill(other_pid,SIGINT);

            break;
        case -1: ;
            //get the pids
            printf("This process's ID: %ld\n", (long) getpid());
            char *line2 = NULL; size_t line2_length = 0;
            printf("First: Enter other process ID: ");
            getline(&line2, &line2_length, stdin);
            other_pid = strtol(line2, NULL, 10);
            free(line2);
            //wait for signals. this must be terminated manually
            while(1){
                struct timespec ts = { .tv_sec = 1234567890, .tv_nsec = 1000000000 };
                nanosleep(&ts, NULL);
            }
            
            break;
    }
    fclose(fptr);
}