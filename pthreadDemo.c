//pthread demo
//creating 2 threads to increment a counter NUMBER_OF_INCREMENTS times each
//not atomic can be interrupted.
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

//globally defined counters
int counter = 0;


//number of times that the counter is incremented
#define NUMBER_OF_INCREMENTS 40

void * calc(void * id){
    //for i until the number of increments..
    for(int i = 0; i < NUMBER_OF_INCREMENTS;i++){
        //..increment the counter
        counter++;
        int threadID = ((intptr_t)id);
        printf("threadid  = %d counter = %d\n",threadID,counter);
    }
}

void main(){

    //define 2 threads
    pthread_t threadIdOne, threadIdTwo;
    
    int idThread1 = 1;
    int idThread2 = 2;


    //initialise the first thread
    pthread_create(&threadIdOne,NULL,calc,(void*)(intptr_t)idThread1);
   

    //initialise the threads
    pthread_create(&threadIdTwo,NULL,calc,(void *)(intptr_t)idThread2);
    

    //when completed join the threads
    pthread_join(threadIdOne,NULL);
    pthread_join(threadIdTwo,NULL);

    //print statement marks that both threads have rejoined the main thread
    printf("The value of the counter is %d\n",counter);
}