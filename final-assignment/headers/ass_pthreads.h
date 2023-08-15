#ifndef ASS_PTHREADS_H
#define ASS_PTHREADS_H


#define mutex_init(x, y) { int error = pthread_mutex_init(x, y); if (error) printf("At pthread_mutex_init(): %s", strerror(error)); }
#define mutex_lock(x) { int error = pthread_mutex_lock(x); if (error) printf("At pthread_mutex_lock(): %s", strerror(error)); }
#define mutex_unlock(x) { int error = pthread_mutex_unlock(x); if (error) printf("At pthread_mutex_unlock(): %s", strerror(error)); }


#endif