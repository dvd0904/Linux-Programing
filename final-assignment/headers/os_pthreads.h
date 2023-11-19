#ifndef OSPTHREADS_H
#define OSPTHREADS_H

#define mutex_init(x, y) { int error = pthread_mutex_init(x, y); if (error) printf("At pthread_mutex_init(): %s", strerror(error)); }
#define mutex_lock(x) { int error = pthread_mutex_lock(x); if (error) printf("At pthread_mutex_lock(): %s", strerror(error)); }
#define mutex_unlock(x) { int error = pthread_mutex_unlock(x); if (error) printf("At pthread_mutex_unlock(): %s", strerror(error)); }
#define mutex_destroy(x) { int error = pthread_mutex_destroy(x); if (error) merror_exit("At pthread_mutex_destroy(): %s", strerror(error)); }
#define cond_init(x, y) { int error = pthread_cond_init(x, y); if (error) merror_exit("At pthread_cond_init(): %s", strerror(error)); }
#define cond_wait(x, y) { int error = pthread_cond_wait(x, y); if (error) merror_exit("At pthread_cond_wait(): %s", strerror(error)); }
#define cond_signal(x) { int error = pthread_cond_signal(x); if (error) merror_exit("At pthread_cond_signal(): %s", strerror(error)); }
#define cond_broadcast(x) { int error = pthread_cond_broadcast(x); if (error) merror_exit("At pthread_cond_broadcast(): %s", strerror(error)); }
#define cond_destroy(x) { int error = pthread_cond_destroy(x); if (error) merror_exit("At pthread_cond_destroy(): %s", strerror(error)); }
#define rwlock_init(x, y) { int error = pthread_rwlock_init(x, y); if (error) merror_exit("At pthread_rwlock_init(): %s", strerror(error)); }
#define rwlock_rdlock(x) { int error = pthread_rwlock_rdlock(x); if (error) merror_exit("At pthread_rwlock_rdlock(): %s", strerror(error)); }
#define rwlock_wrlock(x) { int error = pthread_rwlock_wrlock(x); if (error) merror_exit("At pthread_rwlock_wrlock(): %s", strerror(error)); }
#define rwlock_unlock(x) { int error = pthread_rwlock_unlock(x); if (error) merror_exit("At pthread_rwlock_unlock(): %s", strerror(error)); }
#define rwlock_destroy(x) { int error = pthread_rwlock_destroy(x); if (error) merror_exit("At pthread_rwlock_destroy(" #x "): %s", strerror(error)); }

#endif