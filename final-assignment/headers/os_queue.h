#ifndef OS_QUEUE_H
#define OS_QUEUE_H

#include <pthread.h>

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


typedef struct sdata_ele {
    char msg[256];
    int read_by[READER_THREADS]; 
} sdata_ele_t;


/* Initializes a new queue structure */
os_queue_t * queue_init(size_t n);

/* Frees an existent queue */
void queue_free(os_queue_t * queue);

/* Evaluates whether the queue is full or not */
int queue_full(const os_queue_t * queue);

/* Evaluates whether the queue is empty or not */
int queue_empty(const os_queue_t * queue);

/* Tries to insert an element into the queue */
int queue_push(os_queue_t * queue, void * data);

/* queue_push but with mutual exclusion */
int queue_push_ex(os_queue_t * queue, void * data);

/* Retrieves next item in the queue */
// void * queue_pop(os_queue_t * queue, int read);
int queue_pop(os_queue_t * queue, sdata_ele_t *data, int read);

/* Same as queue_pop but with mutual exclusion */
void * queue_pop_ex(os_queue_t * queue, int read);


#endif