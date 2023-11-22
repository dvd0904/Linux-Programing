#include "shared.h"

static int list_full(os_list *list);
static int list_empty(os_list *list);
static int _list_push(os_list *list, void *data);

os_list *list_create(int size)
{
    os_list *list;

    list = (os_list *)calloc(1, sizeof(os_list));
    if(list == NULL)
        return NULL;
    
    list->first_node = NULL;
    list->last_node = NULL;
    list->cur_size = 0;
    list->max_size = size;

    mutex_init(&list->mutex, NULL);
    mutex_init(&list->cnt_mutex, NULL);
    cond_init(&list->data_available, NULL);

    return list;
}

int list_destroy(os_list *list)
{
    if(!list)
        return LIST_FAIL;

    list_clean_nodes(list);
    
    mutex_destroy(&list->mutex);
    mutex_destroy(&list->cnt_mutex);
    cond_destroy(&list->data_available);

    free(list);
    list = NULL;
    return LIST_SUCCESS;
}

int list_clean_nodes(os_list *list)
{
    
    mutex_lock(&list->mutex);

    if(!list)
    {
        mutex_unlock(&list->mutex);
        return LIST_FAIL;
    }

    if(list_empty(list))
    {
        mutex_unlock(&list->mutex);
        return LIST_NODATA;
    }
    
    os_list_node *temp = NULL;

    while(list->first_node != NULL)
    {
        temp = list->first_node;
        list->first_node = temp->next;
        free(temp);
    }

    list->cur_size = 0;
    list->first_node = NULL;
    list->last_node = NULL;
    
    mutex_unlock(&list->mutex);

    return LIST_SUCCESS;
}


int list_push(os_list *list, void *data)
{
    int retval;
    mutex_lock(&list->mutex);
 
    retval = _list_push(list, data);
    if(!retval)
        cond_broadcast(&list->data_available);
    
    mutex_unlock(&list->mutex);

    return retval;
}

void *list_get_data_first_node(os_list *list)
{
    os_list_node *first = NULL;
    mutex_lock(&list->mutex);
  
    while(first = list->first_node, !first)
        cond_wait(&list->data_available, &list->mutex);
        
    mutex_unlock(&list->mutex);

    if(!first)
    {
        printf("Can not get data from node.\n");
        return NULL;
    }
    return first->data;
}

void *list_get_data_last_node(os_list *list)
{
    mutex_lock(&list->mutex);

    os_list_node *last = list->last_node;
    mutex_unlock(&list->mutex);

    if(!last)
        return NULL;
    return last->data;
}

int list_remove_first_node(os_list *list)
{
    os_list_node *temp;
    mutex_lock(&list->mutex);

    if(list_empty(list))
    {
        mdebug("Shared list is empty.");
        mutex_unlock(&list->mutex);
        return LIST_NODATA;
    }
    else 
    {
        temp = list->first_node->next;
        if(temp)
            temp->prev = NULL;
        else 
            list->last_node = temp;

        free(list->first_node);
        list->first_node = temp;
    }

    list->cur_size--;

    mutex_unlock(&list->mutex);

    return LIST_SUCCESS;
}

void print_list(os_list *list)
{
    
    mutex_lock(&list->mutex);
    if(list_empty(list))
    {
        mutex_unlock(&list->mutex);
        printf("List is empty.\n");
        return;
    }
    os_list_node *temp = list->first_node;
    while(temp != NULL)
    {
        printf("%s ", (char *)(temp->data));
        temp = temp->next;
    }
    mutex_unlock(&list->mutex);
    printf("\n");   
}

static int list_full(os_list *list)
{
    return list->cur_size == list->max_size;
}

static int list_empty(os_list *list)
{
    if(list->cur_size == 0 || list->first_node == NULL)
        return 1;
    return 0;
}

static int _list_push(os_list *list, void *data)
{
    if(list_full(list))
        return LIST_FULL;
    else
    {
        os_list_node *newnode = (os_list_node *)calloc(1, sizeof(os_list_node));
        if(!newnode)
        {
            mdebug(MEM_ERROR, errno, strerror(errno));
            return LIST_FAIL;
        }

        newnode->data = data;

        if(list_empty(list))
        {
            list->first_node = newnode;
            list->last_node = newnode;
        }
        else 
        {
            list->last_node->next = newnode;
            newnode->prev = list->last_node;
            list->last_node = newnode;
        }
        list->cur_size++;
    }

    return LIST_SUCCESS;
}