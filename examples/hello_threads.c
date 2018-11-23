/**
 * Demonstrates more than one threads
 */
#include <pthread.h>
#include <stdio.h>

#define MAX_THREADS 4

// Function executed on separate threads
void *hello(void *tid)
{
    printf("Hello World! Greetings from thread %p\n", tid);
    pthread_exit(NULL);
}

int main(void)
{
    pthread_t tid[MAX_THREADS];

    // create threads
    for (int i = 0; i < MAX_THREADS; i++)
    {
        if (pthread_create(&tid[i], NULL, hello, &tid[i]) != 0)
        {
            fprintf(stderr, "Error creating threads\n");
        }
    }

    // Join threads, i.e., don't exit main thread until all other finishes
    for (int i = 0; i < MAX_THREADS; i++)
    {
        if (pthread_join(tid[i], NULL) != 0)
        {
            fprintf(stderr, "Cannot join threads\n");
        }
    }

    return 0;
}