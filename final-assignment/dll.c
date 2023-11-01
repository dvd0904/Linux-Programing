#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "list.h"

struct dai_t {
    struct list_head head;
    char *data;
};

int main()
{
    struct list_head list;
    struct dai_t *pos, *n;
    const char name[] = "Dai%d";

    INIT_LIST_HEAD(&list);

    struct dai_t *obj;
    for (int i = 0; i < 10; i++) {
        obj = malloc(sizeof(*obj));
        obj->data = calloc(32, 1);
        sprintf(obj->data, name, i);
        list_add(&obj->head, &list);
    }

    list_for_each_entry(pos, &list, head) {
        printf("%s\n", pos->data);
    }

    list_for_each_entry_safe(pos, n, &list, head) {
        list_del(&pos->head);
        free(pos);
    }
}