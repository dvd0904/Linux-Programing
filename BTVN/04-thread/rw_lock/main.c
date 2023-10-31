#include <pthread.h>
#include <stdio.h>

#define SIZE 5
int a[SIZE] = {1, 2, 3, 4, 5};
int check[2] = {0, 0};
pthread_rwlock_t lock; // Khóa đọc-viết

void *read_array1(void *arg) {
    pthread_rwlock_rdlock(&lock); 
    printf("%d\n", a[0]);
    *(int *)arg = 1;
    pthread_rwlock_unlock(&lock);
    return NULL;
}

void *read_array2(void *arg) {
    pthread_rwlock_rdlock(&lock); 
    printf("%d\n", a[0]);
    *(int *)arg = 1;
    pthread_rwlock_unlock(&lock);
    return NULL;
}

int all()
{
    for(int i = 0; i < 2; i++)
    {
        if(!check[i])
            return 0;
    }
    return 1;
}

int main() {
    pthread_t thread1, thread2;

    if(pthread_rwlock_init(&lock, NULL) != 0) {
        printf("Không thể khởi tạo khóa đọc-viết.\n");
        return 1;
    }

    pthread_create(&thread1, NULL, read_array1, &check[0]);
    pthread_create(&thread2, NULL, read_array2, &check[1]);

    // if(all())
        a[0] = 10;

    printf("%d\n", a[0]);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    pthread_rwlock_destroy(&lock);

    return 0;
}
