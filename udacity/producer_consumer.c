/**
 * Producer Consumer problem
 */

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Buffer size
#define MAX_BUFFER 6

// Shared buffer
int buffer[MAX_BUFFER];

// Mutex lock
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Shared variables for buffer
int buffer_size = 0;
int buffer_start = 0;
int buffer_end = 0;

// Condition variables for producer and consumer threads
pthread_cond_t cond_prod = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_cons = PTHREAD_COND_INITIALIZER;

// Prototypes
void *producer(void *arg);
void *consumer(void *arg);

int main(void)
{
    pthread_t prod_id;
    pthread_t cons_id;

    // Open producer thread
    if (pthread_create(&prod_id, NULL, producer, NULL) != 0)
    {
        fprintf(stderr, "Could not open producer thread\n");
        exit(-1);
    }

    // Open consumer thread
    if (pthread_create(&cons_id, NULL, consumer, NULL) != 0)
    {
        fprintf(stderr, "Could not create consumer thread\n");
        exit(-1);
    }

    // Wait for threads to complete before exiting process
    pthread_join(prod_id, NULL);
    pthread_join(cons_id, NULL);

    return 0;
}

// Producer thread. It adds item to the buffer
void *producer(void *arg)
{
    // produce 10 items
    for (int i = 0; i < 10; i++)
    {
        // lock thread
        pthread_mutex_lock(&mutex);

        // wait if buffer is full
        while (buffer_size == MAX_BUFFER)
            pthread_cond_wait(&cond_prod, &mutex);

        // update buffer
        buffer[buffer_end] = i;
        buffer_end = (buffer_end + 1) % MAX_BUFFER;
        buffer_size++;
        
        // unlock thread
        pthread_mutex_unlock(&mutex);
        
        // signal consumer
        printf("Item produced: %d\n", i);
        pthread_cond_signal(&cond_cons);
    }

    printf("Producer: Out of stock\n");

    pthread_exit(NULL);
}

// Consumer thread. It removes item from the buffer.
void *consumer(void *arg)
{
    // consume item
    while (true)
    {
        // lock thread
        pthread_mutex_lock(&mutex);

        // wait if buffer is empty
        while (buffer_size == 0)
            pthread_cond_wait(&cond_cons, &mutex);

        // consume item
        int i = buffer[buffer_start];
        buffer_start = (buffer_start + 1) % MAX_BUFFER;
        buffer_size--;

        printf("Item consumed: %d\n", i);
        pthread_cond_signal(&cond_prod);

        // unlock thread
        pthread_mutex_unlock(&mutex);
    }

    printf("Consumer: Can't take it anymore\n");

    pthread_exit(NULL);
}