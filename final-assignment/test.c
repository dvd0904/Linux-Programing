#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct child
{
    int d;
    int s;
    int k;
};

struct parent
{
    char c;
    double d;
    int s;
    struct child x;
};

typedef struct queue_t {
    void ** data;                           /* Pointer to the circular buffer */
    size_t begin;                           /* Stores the index of the next empty space */
    size_t end;                             /* Stores the index of the next element */
    size_t size;                            /* Size of the queue */
    pthread_mutex_t mutex;                  /* mutex for mutual exclusion */
    pthread_cond_t available;               /* condition variable when queue is empty */
    pthread_cond_t available_not_empty;     /* Condition variable when queue is full */
    unsigned int elements;                  /* counts the number of elements stored in the queue */
} os_queue_t;

int main()
{
    printf("size of: %ld\n", sizeof(void**));
    printf("size of: %ld\n", sizeof(size_t));
    printf("size of: %ld\n", sizeof(pthread_cond_t));
    printf("size of: %ld\n", sizeof(pthread_mutex_t));
    printf("size of: %ld\n", sizeof(unsigned int));
   
    // printf("size: %ld\n", sizeof(struct child));
    // printf("size: %ld\n", sizeof(struct parent));
    // printf("size: %ld\n", sizeof(os_queue_t));

    return 0;
}