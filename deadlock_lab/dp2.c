#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <unistd.h>

pthread_barrier_t barrier; // optional: to hopefully make deadlock more consistent

pthread_t philosopher[5];
pthread_mutex_t chopstick[5];

void *eat(void *arg) {
    int n = (int) (long)arg;
    int first = n;
    int second = (n+1)%5;
    if(first > second){
        int temp = first;
        first = second;
        second = temp;
    }
    // optional: sync up threads to make deadlock hopefully happen more consistently
    pthread_barrier_wait(&barrier);

    // take two chopsticks
    pthread_mutex_lock(&chopstick[first]);
    printf("Philosopher %d got chopstick %d\n", first, first);
    pthread_mutex_lock(&chopstick[second]);
    printf("Philosopher %d got chopstick %d\n", first, second);
    
    printf ("Philosopher %d is eating\n",first);
    sleep(1);
    
    // set them back down
    printf("Philosopher %d set down chopstick %d\n", first, second);
    pthread_mutex_unlock(&chopstick[second]);
    printf("Philosopher %d set down chopstick %d\n", first, first);
    pthread_mutex_unlock(&chopstick[first]);
    return NULL;
}

int main(int argc, const char *argv[]) {
    pthread_barrier_init(&barrier, NULL, 5);

    for(int i = 0; i < 5; i += 1)
        pthread_mutex_init(&chopstick[i], NULL);

    for(int i =0; i < 5; i += 1)
        pthread_create(&philosopher[i], NULL, eat, (void *)(size_t)i);

    for(int i=0; i < 5; i += 1)
        pthread_join(philosopher[i], NULL);

    for(int i=0; i < 5; i += 1)
        pthread_mutex_destroy(&chopstick[i]);

    pthread_barrier_destroy(&barrier);

    return 0;
}