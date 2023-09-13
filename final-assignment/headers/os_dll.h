#ifndef FUNC_H
#define FUNC_H

#include "shared.h"
#include "os_net/os_net.h"

typedef struct dbl_list
{
    void *data;
    struct dbl_list *prev;
    struct dbl_list *next;
} dbl_list_t;

dbl_list_t *dbl_list_create(void *data);

int dbl_list_is_empty(dbl_list_t *head);

void dbl_list_show(dbl_list_t *head);

int dbl_list_size(dbl_list_t *head);

void dbl_list_insert_begin(dbl_list_t **head, void *data);

void dbl_list_insert_end(dbl_list_t **head, void *data);

void dbl_list_insert_middle(dbl_list_t **head, void *data, int idx);

void dbl_list_delete_begin(dbl_list_t **head);

void dbl_list_delete_end(dbl_list_t **head);

void dbl_list_delete_middle(dbl_list_t **head, int idx);

dbl_list_t *dbl_list_sort(dbl_list_t *head);

dbl_list_t *dbl_list_get_node(dbl_list_t *socket_list, tcpsock_el_t *dummy);

#endif