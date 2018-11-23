/**
 * Demonstrates the use of pthread_attr_* 
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// main thread
void *foo(void *arg)
{
    printf("Foobar!\n");
    pthread_exit(NULL);
}

int main(void)
{
    // Stores thread id when thread is created
    pthread_t tid;

    pthread_attr_t attr;

    // initialize thread attrbute object
    int r = pthread_attr_init(&attr);
    if (r != 0)
    {
        fprintf(stderr, "Failed to created thread attribute object. Error %d\n", r);
        exit(-1);
    }

    // Create thread in detached state
    r = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (r != 0)
    {
        fprintf(stderr, "Error in setdetachstate. Code = %d\n", r);
        exit(-1);
    }

    // Set thread scope to system
    r = pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
    if (r != 0)
    {
        fprintf(stderr, "Error in setscope. Code = %d\n", r);
        exit(-1);
    }

    // Create thread
    r = pthread_create(&tid, &attr, foo, NULL);
    if (r != 0)
    {
        fprintf(stderr, "Failed to create thread. Code = %d\n", r);
        exit(-1);
    }

    pthread_exit(NULL);
}