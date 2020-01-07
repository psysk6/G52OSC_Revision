//example of the single producer single consumer problem using semaphores and how to prevent it from deadlocking.
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>






//TODO CHANGE THIS TO A FOR LOOP TO MAKE IT ADAPTABLE FOR LATER
//number of times that the counter is incremented
#define NUMBER_OF_PRODUCERS 1 
#define NUMBER_OF_CONSUMERS 1 
#define NUMBER_TO_PRODUCE 500


//number of items the producer has created
int numberProduced  = 0;
int numberConsumed = 0;
//size of the buffer
int buffer = 0;
//numberConsumed
//exit when the number consumed = the number its meant to post


//create the semaphores
//semaphore semSync for syncing the 2 processes semDelayConsumer to prevent  
sem_t semSync, semDelayConsumer;



//code to be executed by the consumer thread.
void * consumer(void * id){
   while(1){
       int threadID = ((intptr_t)id);
       if(numberConsumed == NUMBER_TO_PRODUCE){
           printf("exiting consumer\n");
           pthread_exit(0);
       }
       else{
            //if buffer is full..
            if(buffer == 0){
                //..consumer does not need cpu time goes to sleep..
                sem_post(&semDelayConsumer);
            }
            else{
                //enter critical section..
                sem_wait(&semSync);
                numberConsumed++;
                buffer--;
                printf("consumer id = %d buffer size = %d\n",threadID,buffer);
                //..exit critical section
                
                sem_post(&semSync);
            }
       }
       sem_post(&semDelayConsumer);
   }
   
}


//code to be executed by the producer thread.
void * producer(void * id){
    //prevent the consumer from attempting to consume from the empty buffer (would seg fault)
    int threadID = ((intptr_t)id);
    sem_wait(&semDelayConsumer);
    //infinitely loop the consumer to produce
    while(1){

        sem_wait(&semSync);
        //if the producer has produced the maximum number of items..
        if(numberProduced == NUMBER_TO_PRODUCE){
            //..exit the thread (with the successful message)
            printf("exiting producer\n");
            pthread_exit(0);
        }
        //..else the producer.. 
        else{
            numberProduced++;
            buffer++;
            printf("producer id = %d buffer size = %d\n",threadID,buffer);
            //allow consumer to enter critical section
            sem_post(&semSync);
            sem_post(&semDelayConsumer);
        }
    }

}


void main(){


    sem_init(&semSync, 0, 1);
    //define 2 threads
    pthread_t threadProducer, threadConsumer;
    
    int idProducerThread = 0;
    int idConsumerThread = 1;


    //initialise the first thread
    pthread_create(&threadProducer,NULL,producer,(void*)(intptr_t)idProducerThread);
   

    //initialise the threads
    pthread_create(&threadConsumer,NULL,consumer,(void *)(intptr_t)idConsumerThread);
    

    //when completed join the threads
    pthread_join(idProducerThread,NULL);
    pthread_join(idConsumerThread,NULL);

    //print statement marks that both threads have rejoined the main thread
    printf("number produced =%d number consumed = %d max produced = %d",numberProduced,numberConsumed,NUMBER_TO_PRODUCE);
}