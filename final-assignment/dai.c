#include <pthread.h>
#include <sys/wait.h>
#include "shared.h"
#include "cJSON.h"
#include "os_net/os_net.h"

#define TEST "test%d"


sdata_t *shared;


void *func(void *arg)
{
    char msg[256];
    memset(msg, '\0', sizeof(msg));
    int x = *(int *)arg;

    sdata_pop(shared, msg, x);
    printf("At thread %d --- msg: %s\n", x, msg);

}

int main()
{

    pthread_t t1, t2;
    use_debug();


    shared = sdata_init(10);

    sdata_ele_t ele[5];
    for(int i = 0; i < 5; i++)
    {
        sprintf(ele[i].msg, TEST, i);
        sdata_push(shared, ele[i].msg);
    }

    int a1 = 0, a2 = 1;


    pthread_create(&t1, NULL, &func, &a1);
    pthread_create(&t2, NULL, &func, &a2);


    sleep(4);

    sdata_print(shared);


    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    sdata_free(shared);

    return 0;
}