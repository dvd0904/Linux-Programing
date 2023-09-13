#include "shared.h"
#include "os_net/os_net.h"

static void dbl_list_front_back_split(dbl_list_t *source, dbl_list_t **front, dbl_list_t **back);
static int cmp(void *x, void *y);

dbl_list_t *dbl_list_create(void *data)
{
    dbl_list_t *head = (dbl_list_t *)malloc(sizeof(dbl_list_t));
    head->prev = NULL;
    head->next = NULL;
    head->data = data;
    return head;
}


int dbl_list_is_empty(dbl_list_t *head)
{
    return head == NULL;
}

void dbl_list_show(dbl_list_t *head)
{
    while(head != NULL)
    {
        printf("%d  ", *((int *)head->data));
        head = head ->next;
    }

    
    printf("\n");
}

int dbl_list_size(dbl_list_t *head)
{
    int cnt = 0;
    while(head != NULL)
    {
        cnt++;
        head = head->next;
    }

    return cnt;
}


void dbl_list_insert_begin(dbl_list_t **head, void *data)
{
    dbl_list_t *dummy = dbl_list_create(data);

    if(dbl_list_is_empty(*head))
        *head = dummy;
    else
    {
        dummy->next = *head;
        (*head)->prev = dummy;
        *head = dummy;
    }
}

void dbl_list_insert_end(dbl_list_t **head, void *data)
{

    dbl_list_t *dummy = dbl_list_create(data);
    if(dbl_list_is_empty(*head))
        *head = dummy;
    else 
    {
        dbl_list_t *tmp = *head;
        while(tmp ->next != NULL)
            tmp = tmp->next;

        tmp->next = dummy;
        dummy->prev = tmp;
    }
}

void dbl_list_insert_middle(dbl_list_t **head, void *data, int idx)
{
    dbl_list_t *dummy = dbl_list_create(data);
    if(dbl_list_is_empty(*head))
    {
        *head = dummy;
        return;
    }
    if(idx <= 1)
    {
        dbl_list_insert_begin(head, data);
        return;
    }
    // if(idx >= dbl_list_size(*head))
    // {
    //     dbl_list_insert_end(head, data);
    //     return;
    // }
    // for(int i = 1; i <= idx - 2; i++)
    //     tmp = tmp->next;

    dbl_list_t *tmp = *head;

    while(idx != 1)
    {
        tmp = tmp->next;
        idx--;
    }

    if(tmp->next == NULL) // last
    {
        tmp->next = dummy;
        dummy->prev = tmp;
    }
    else 
    {
        dbl_list_t *tmp2 = tmp->next;
        tmp->next = dummy;
        tmp2->prev = dummy;
        dummy->prev = tmp;
        dummy->next = tmp2;
    }
}


void dbl_list_delete_begin(dbl_list_t **head)
{
    if(dbl_list_is_empty(*head))
        return;
    
    *head = (*head)->next;
    free((*head)->prev);
    (*head)->prev = NULL;
}

void dbl_list_delete_end(dbl_list_t **head)
{
    dbl_list_t *tmp = *head;
    while(tmp->next != NULL)
        tmp = tmp->next;

    tmp->prev->next = NULL;
    free(tmp);
    tmp = NULL;
}

void dbl_list_delete_middle(dbl_list_t **head, int idx)
{
    dbl_list_t *tmp = *head;

    if(idx == 1)
    {
        dbl_list_delete_begin(head);
        return;
    }

    while(idx > 1)
    {
        tmp = tmp->next;
        idx--;
    }

    if(tmp->next == NULL) // last
        dbl_list_delete_end(head);
    else 
    {
        dbl_list_t *tmp2 = tmp->prev;

        tmp2->next = tmp->next;
        tmp->next->prev = tmp2;

        free(tmp);
        tmp = NULL;
    }

}


static void dbl_list_front_back_split(dbl_list_t *source, dbl_list_t **front, dbl_list_t **back)
{
    dbl_list_t *fast = NULL, *slow = NULL;

    slow = source;
    fast = source->next;

    while(fast != NULL)
    {
        fast = fast->next;
        if(fast != NULL)
        {
            slow = slow->next;
            fast = fast->next;
        }
    }

    /* Slow here is midpoint => split */
    *front = source;
    *back = slow->next;
    slow->next = NULL;
}

static dbl_list_t *merge(dbl_list_t *front, dbl_list_t *back)
{
    if(!front) return back;
    if(!back)  return front;

    if(*((int *) front->data) < *((int *)back->data))
    {
        front->next = merge(front->next, back);
        front->next->prev = front;
        front->prev = NULL;
        return front;
    }

    back->next = merge(front, back->next);
    back->next->prev = back;
    back->prev = NULL;
    return back;
    
}


dbl_list_t *dbl_list_sort(dbl_list_t *head)
{
    if(!head || !head->next)
    {
        return head;
    }   

    dbl_list_t *first = NULL, *second = NULL;
    dbl_list_front_back_split(head, &first, &second);

    first = dbl_list_sort(first);
    second = dbl_list_sort(second);

    return merge(first, second);

}

static int cmp(void *x, void *y)
{
    
    if(((tcpsock_el_t *) x)->sd > ((tcpsock_el_t *) y)->sd)
        return 1;
    if(((tcpsock_el_t *) x)->sd == ((tcpsock_el_t *) y)->sd)
        return 0;
    return -1;
}

dbl_list_t *dbl_list_get_node(dbl_list_t *socket_list, tcpsock_el_t *dummy)
{
    if(dbl_list_is_empty(socket_list)) return NULL;
    dbl_list_t *tmp = socket_list;
    while(tmp->next != NULL && cmp(dummy, socket_list->data) != 0)
    {
        tmp = tmp->next;
    }
    return tmp;
}
