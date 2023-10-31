#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define THRESHOLD 5

pthread_t tid;

int counter;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

static void *threadFunc(void *args)
{
    printf("LINE: %d ----- Locking mutex...\n", __LINE__);
    pthread_mutex_lock(&mutex);
    printf("LINE: %d ----- Mutex locked!\n", __LINE__);
    printf("pthread has started\n");

    while (counter < THRESHOLD)
    {
        counter += 1;
        sleep(1);
    }

    printf("Sending signal...\n");
    pthread_cond_signal(&cond);
    printf("pthread has finished, counter = %d\n", counter);
    pthread_mutex_unlock(&mutex);

    printf("LINE: %d ----- Mutex is free\n", __LINE__);

    pthread_exit(NULL);
}

int main(void)
{
    int ret = 0;

    ret = pthread_create(&tid, NULL, threadFunc, NULL);
    if (ret != 0)
    {
        printf("pthread created fail\n");
    }

    printf("LINE: %d ----- Locking mutex...\n", __LINE__);
    pthread_mutex_lock(&mutex);

    printf("LINE: %d ----- Mutex locked!\n", __LINE__);
    while (counter < THRESHOLD)
    {
        printf("debug\n");
        pthread_cond_wait(&cond, &mutex);
        printf("Global variable counter = %d.\n", counter);
    }

    printf("Unlocking mutex...\n");

    pthread_mutex_unlock(&mutex);
    pthread_join(tid, NULL);

    return 0;
}