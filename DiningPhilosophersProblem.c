#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define N 5
#define THINKING 1
#define HUNGRY 2
#define EATING 3
int state[N] = {THINKING, THINKING, THINKING, THINKING, THINKING};
sem_t phil[N]; // sends philosopher to sleep
sem_t sync;
void * philosopher(void * id)
{
    int i = *((int *) id);
    while(1)
    {
        printf("%d is thinking\n", i);
        take_forks(i);
        printf("%d is eating\n", i);
        put_forks(i);
    }
}

void take_forks(int i)
{
    sem_wait(&sync);
    state[i] = HUNGRY;
    test(i);
    sem_post(&sync);
    sem_wait(&phil[i]);
}

void test(int i)
{
    int left = (i + N - 1) % N;
    int right = (i + 1) % N;
    if(state[i] == HUNGRY && state[left] != EATING&& state[right] != EATING) {
        state[i] = EATING;
        sem_post(&phil[i]);
    }
}

void put_forks(int i)
{
    int left = (i + N - 1) % N;
    int right = (i + 1) % N;
    sem_wait(&sync);
    state[i] = THINKING;
    test(left);
    test(right);
    sem_post(&sync);
}
void test(int i)
{
    int left = (i + N - 1) % N;
    int right = (i + 1) % N;
    if(state[i] == HUNGRY && state[left] != EATING && state[right] != EATING) {
        state[i] = EATING;
        sem_post(&phil[i]);
    }
}

void main(){
    //todo
}