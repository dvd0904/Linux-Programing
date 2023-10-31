#ifndef SS_H
#define SS_H

#include "os_defs.h"

struct shared_data
{
    char *msg;
    int read_by[READER_THREADS];
};

/* Merge sort */
void sort(int *arr, int l, int r);

/* Binary search */
int search(int *a, int x, int left, int right);

/* Calculate avg of temp value */
int avg(int *a);

#endif