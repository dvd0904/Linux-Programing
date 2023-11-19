#ifndef LIST_H
#define LIST_H

#define LIST_SUCCESS       0
#define LIST_FAIL          -1
#define LIST_NODATA        -2
#define LIST_FULL          -3

typedef struct _os_list_node
{
    struct _os_list_node *next;
    struct _os_list_node *prev;
    void *data;
} os_list_node;

typedef struct _os_list
{
    os_list_node *first_node;
    os_list_node *last_node;

    int cur_size;
    int max_size;

    pthread_mutex_t mutex;
    pthread_cond_t data_available;
} os_list;

os_list *list_create(int size);

/**
 * @brief Frees all resources associated with a list.
 * @param list List to be destroyed.
*/
int list_destroy(os_list *list);

/**
 * @brief Clears all the nodes from a list and frees the referenced data
 * @param list List to delete
 */
int list_clean_nodes(os_list *list);

/**
 * @brief Insert data at the end of a list
 * @param list List where the node will be inserted into
 * @param data Data to be inserted
 * @return 0 if succsess otherwise fail
 */
int list_push(os_list *list, void *data);

/**
 * @brief Read the first node of list
 * @param list List where the node is stored
 * @return data first node
 */
void *list_get_data_first_node(os_list *list);


/**
 * @brief Read the last node of list
 * @param list List where the node is stored
 * @return data last node
 */
void *list_get_data_last_node(os_list *list);

/**
 * @brief Remove the first node of list
 * @param list List where the node is stored
 * @return first node
 */

int list_remove_first_node(os_list *list);

/**
 * @brief print data of a list
 * @param list List to print
 */
void print_list(os_list *list);

#endif