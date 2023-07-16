#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

pthread_t thread_id1, thread_id2;

typedef struct
{
    int age;
    char name[30];
    char msg[30];
} thr_data_t;

static void *thr_handle(void *args)
{
    pthread_t tid = pthread_self();
    thr_data_t *data = (thr_data_t *)args;

    if (pthread_equal(tid, thread_id1))
    {
        printf("I'm thread_id1\n\n");
    }
    else
    {
        printf("I'm thread_id2\n");
        printf("Hello %s, I'm %d, %s\n", data->name, data->age, data->msg);
    }
    int i = 0;
    for (;;)
    {
        printf("handle1111111111111111111111111111111111111111111111111: %d\n", i);
        i++;
        if (i == 10)
        {
            // printf("call exit\n");
            // exit(0);
            pthread_cancel(thread_id1);
            break;
        }
        sleep(1);
    }

    printf("Bai\n");
    // exit(0);
}

static void *thr_handle2(void *args)
{
    printf("Hi im thread handle2\n");
    int i = 0;
    for (;;)
    {
        printf("handle2: %d\n", i);
        i++;
        sleep(1);
    }
}

int main(int argc, char const *argv[])
{
    /* code */
    int ret;
    thr_data_t data = {0};

    strncpy(data.name, "dai", sizeof(data.name));
    strncpy(data.msg, "Konichiwa\n", sizeof(data.msg));
    data.age = 22;

    // sleep(2);

    if (ret = pthread_create(&thread_id2, NULL, &thr_handle, &data))
    {
        printf("pthread_create() error number=%d\n", ret);
        return -1;
    }

    // sleep(2);

    if (ret = pthread_create(&thread_id1, NULL, &thr_handle2, NULL))
    {
        printf("pthread_create() error number=%d\n", ret);
        return -1;
    }

    // pthread_join(thread_id1, NULL);
    // pthread_join(thread_id2, NULL);

    while (1)
    {
        printf("main\n");
        sleep(1);
    }

    return 0;
}
