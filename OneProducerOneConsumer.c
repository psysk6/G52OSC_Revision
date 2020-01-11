//example of the single producer single consumer problem using semaphores and how to prevent it from deadlocking.
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>






//TODO CHANGE THIS TO A FOR LOOP TO MAKE IT ADAPTABLE FOR LATER
//number of times that the counter is incremented
#define NUMBER_OF_PRODUCERS 1 
#define NUMBER_OF_CONSUMERS 1 
#define NUMBER_TO_PRODUCE 1000


//number of items the producer has created
int numberProduced  = 0;
int numberConsumed = 0;
//size of the buffer
int buffer = 0;
//numberConsumed
//exit when the number consumed = the number its meant to post
int temp;

//create the semaphores
//semaphore semSync for syncing the 2 processes semDelayConsumer to prevent  
sem_t semSync, semDelayConsumer;



//code to be executed by the consumer thread.
void * consumer(void * id){

    //threadID is the ID of the thread- this will become more useful later on when I do multiple producers & consumers
    int threadID = ((intptr_t)id);
    sem_wait(&semDelayConsumer);
    while(1){
        sem_wait(&semSync);
        if(numberConsumed == NUMBER_TO_PRODUCE){
            sem_post(&semDelayConsumer);
            sem_post(&semSync);
            printf("exit");
            pthread_exit(0);
        }
        if(buffer >= 1){
            buffer--;
            numberConsumed++;
            temp = buffer;
            printf("consumer id = %d, buffer = %d\n",threadID,buffer);
            sem_post(&semSync);
            if(temp >= 0){
                sem_post(&semDelayConsumer);
            }
        }
        else{
            sem_post(&semSync);
            sem_post(&semDelayConsumer);
        }
      
    }

}


//code to be executed by the producer thread.
void * producer(void * id){
    int threadID = ((intptr_t)id);
    while(1){
        //wait the sync semaphore-> enter critical section...
        sem_wait(&semSync);
        //if the producer has produced the correct number..
        if(numberProduced == NUMBER_TO_PRODUCE){
            printf("exiting producer\n");
            //..exit the thread
            pthread_exit(0);
        }
        //else..
        //increment the buffer
        buffer++;
        //keep track of the number produced
        numberProduced++;
        printf("producer id = %d, buffer = %d\n",threadID,buffer);
        
        //if there is at least one item in the buffer..
        if(buffer >=1){
            //..wake up the consumer give it an opportunity to consume..
            printf("waking up the consumer\n");
            sem_post(&semDelayConsumer);   
        }
        //..exit the critical section
        sem_post(&semSync);
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