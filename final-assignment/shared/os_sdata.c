#include "shared.h"


static int read_by_all(sdata_node_t *node)
{
    for(int i = 0; i < READER_THREADS; i++)
    {
        if(node->element.read_by[i] == 0)
            return 0;
    }

    return 1;
}

sdata_t *sdata_init(int size)
{
    sdata_t *list;
    os_calloc(1, sizeof(sdata_t), list);
    if(list == NULL)
    {
        merror(MEM_ERROR, errno, strerror(errno));
        return NULL;
    }
    list->first_node = NULL;
    list->last_node = NULL;
    list->cur_size = 0;
    list->max_size = size;

    rwlock_init(&list->lock, NULL);

    return list;
}


int sdata_free(sdata_t *list)
{
    rwlock_wrlock(&list->lock);
    if(list == NULL)
    {
        rwlock_unlock(&list->lock);
        return SDATA_FAIL;
    }

    while(sdata_remove_first(list) != SDATA_NODATA);

    list->first_node = NULL;
    list->last_node = NULL;
    list->cur_size = 0;
    list->max_size = 0;

    rwlock_unlock(&list->lock);
    rwlock_destroy(&list->lock);

    os_free(list);
    list = NULL;

    return SDATA_SUCCESS;
}

int sdata_remove_first(sdata_t *list)
{
    if(list == NULL)
        return SDATA_FAIL;

    /* If list does not have any node */
    if(list->first_node == NULL)
        return SDATA_NODATA;

    sdata_node_t *temp = list->first_node;

    /* If list has only one node */
    if(list->first_node == list->last_node)
        list->first_node = list->last_node = NULL;
    else
    {
        list->first_node = list->first_node->next;
        list->first_node->prev = NULL;
    }

    list->cur_size--;
    
    temp->next = NULL;
    temp->prev = NULL;

    os_free(temp);

    return SDATA_SUCCESS;
    
}


int sdata_push(sdata_t *list, char *data)
{
    if(list == NULL)
        return SDATA_FAIL;
    
    sdata_node_t *temp = malloc(sizeof(sdata_node_t));
    if(!temp)
    {
        merror(MEM_ERROR, errno, strerror(errno));
        return SDATA_FAIL;
    }

    strcpy(temp->element.msg, data);
    temp->next = NULL;
    temp->prev = NULL;

    for(int i = 0; i < READER_THREADS; i++)
        temp->element.read_by[i] = 0;

    rwlock_wrlock(&list->lock);

    /* If list does not have any node */
    if(list->first_node == NULL)
    {
        list->first_node = temp;
        list->last_node = temp;
    }
    else
    {
        list->last_node->next = temp;
        temp->prev = list->last_node;
        list->last_node = temp;
    }

    rwlock_unlock(&list->lock);

    return SDATA_SUCCESS;

}


int sdata_pop(sdata_t *list, char *msg, int read)
{
    if(list->first_node == NULL)
        return SDATA_FAIL;

    rwlock_wrlock(&list->lock);

    if(list->first_node == NULL)
    {
        rwlock_unlock(&list->lock);
        return SDATA_NODATA;
    }

    strcpy(msg, list->first_node->element.msg);
    list->first_node->element.read_by[read] = 1;

    if(read_by_all(list->first_node))
    {
        if(sdata_remove_first(list) < 0)
            mdebug("Can not remove first node");

        mdebug("First node is removed by thread: %d", read);
    }

    rwlock_unlock(&list->lock);

    return SDATA_SUCCESS;
}

static void _sdata_print(sdata_t *list)
{
    sdata_node_t *temp = list->first_node;
    while(temp != NULL)
    {
        printf("%s\n", temp->element.msg);
        temp = temp->next;
    }
    printf("\n");

}

void sdata_print(sdata_t *list)
{
    rwlock_rdlock(&list->lock);
    _sdata_print(list);
    rwlock_unlock(&list->lock);

}