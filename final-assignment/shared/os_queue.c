#include "shared.h"

os_queue_t * queue_init(size_t size) {

    os_queue_t * queue;
    os_calloc(1, sizeof(os_queue_t), queue);
    os_malloc(size * sizeof(void *), queue->data);
    queue->size = size;
    queue->elements = 0;
    mutex_init(&queue->mutex, NULL);
    cond_init(&queue->available, NULL);
    cond_init(&queue->available_not_empty, NULL);
    return queue;
}

void queue_free(os_queue_t * queue) {
    if (queue) 
    {
        free(queue->data);
        mutex_destroy(&queue->mutex);
        cond_destroy(&queue->available);
        cond_destroy(&queue->available_not_empty);
        free(queue);
    }
}

int queue_full(const os_queue_t * queue) {
    return (queue->begin + 1) % queue->size == queue->end;
}

int queue_empty(const os_queue_t * queue) {
    return queue->begin == queue->end;
}

int queue_push(os_queue_t * queue, void * data) {
    if (queue_full(queue)) 
        return -1;
    else 
    {
        queue->data[queue->begin] = data;
        queue->begin = (queue->begin + 1) % queue->size;
        queue->elements++;
        return 0;
    }
}

int queue_push_ex(os_queue_t * queue, void * data) {
    int result;

    mutex_lock(&queue->mutex);

    while (result = queue_full(queue), result) 
        cond_wait(&queue->available_not_empty, &queue->mutex);
    

    result = queue_push(queue,data);

    cond_signal(&queue->available_not_empty);
    cond_signal(&queue->available);
    mutex_unlock(&queue->mutex);

    return result;
}

void * queue_pop(os_queue_t * queue) {
    void * data;

    if (queue_empty(queue)) 
        return NULL;
    else 
    {
        data = queue->data[queue->end];
        queue->end = (queue->end + 1) % queue->size;
        queue->elements--;
        return data;
    }
}

void * queue_pop_ex(os_queue_t * queue) {
    void * data;

    mutex_lock(&queue->mutex);

    while (data = queue_pop(queue), !data) 
        cond_wait(&queue->available, &queue->mutex);
    

    cond_signal(&queue->available_not_empty);
    mutex_unlock(&queue->mutex);

    return data;
}
