// #ifndef OS_LIST_H
// #define OS_LIST_H

// #define SDATA_FAIL          -1
// #define SDATA_NODATA        -2
// #define SDATA_SUCCESS       1

// struct sdata_ele {
//     char msg[256];
//     int read_by[READER_THREADS]; 
// };

// typedef struct sdata_ele sdata_ele_t;

// typedef struct sdata_node {
//     struct sdata_node * next;
//     struct sdata_node * prev;
//     sdata_ele_t element;
// } sdata_node_t;

// struct sdata {
//     sdata_node_t *first_node;
//     sdata_node_t *last_node;

//     int max_size;
//     int cur_size;

//     pthread_rwlock_t lock;
// };

// typedef struct sdata sdata_t;

// sdata_t *sdata_init(int size);

// int sdata_remove_first(sdata_t *list);

// int sdata_free(sdata_t *list);

// int sdata_push(sdata_t *list, char *data);

// void sdata_print(sdata_t *list);

// int sdata_pop(sdata_t *list, char *msg, int read);





// #endif