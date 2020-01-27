//pthread demo
//creating 2 threads to increment a counter NUMBER_OF_INCREMENTS times each
//not atomic can be interrupted.
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

//globally defined counters
int counter = 0;
sem_t semSync;

//number of times that the counter is incremented
#define NUMBER_OF_INCREMENTS 1000

void *calc(void *id)
{
    while(1){
        //fetch the thread ID
        int threadID = ((intptr_t)id);
        sem_wait(&semSync);
        //if reached the number of increments..
        if (counter == NUMBER_OF_INCREMENTS)
        {
            //..exit the thread
            printf("ciao from thread %d\n",threadID);
            pthread_exit(0);
        }
        else{    
            //increment counter
            counter++;
            //print out thread id and counter
            printf("threadid  = %d counter = %d\n", threadID, counter);
            //exit thread's critical section and potentially allow other thread to take control of the cpu
            sem_post(&semSync);
            if (counter == NUMBER_OF_INCREMENTS)
            {
                printf("ciao from thread %d\n",threadID);
                pthread_exit(0);
            }
        }
    }
        
}

void main()
{

    sem_init(&semSync, 0, 1);
    //define 2 threads
    pthread_t threadIdOne, threadIdTwo;

    int idThread1 = 1;
    int idThread2 = 2;

    //initialise the first thread
    pthread_create(&threadIdOne, NULL, calc, (void *)(intptr_t)idThread1);

    //initialise the threads
    pthread_create(&threadIdTwo, NULL, calc, (void *)(intptr_t)idThread2);

    //when completed join the threads
    pthread_join(threadIdOne, NULL);
    pthread_join(threadIdTwo, NULL);

    //print statement marks that both threads have rejoined the main thread
    printf("The value of the counter is %d\n", counter);
}