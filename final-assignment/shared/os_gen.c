#include "shared.h"

static void merge(int *a, int l, int m, int r)
{
    int len1 = m - l + 1;
    int len2 = r - m;
    int L[len1], R[len2];

    for(int i = 0; i < len1; i++)
        L[i] = a[l + i];

    for(int j = 0; j < len2; j++)
        R[j] = a[m + 1 + j];

    int i = 0, j = 0;
    while (i < len1 && j < len2)
    {
        if (L[i] <= R[j])
            a[l++] = L[i++];
        else
            a[l++] = R[j++];
    }

    while (i < len1)
        a[l++] = L[i++];

    while (j < len2)
        a[l++] = R[j++];
}

void sort(int *arr, int l, int r)
{
    if (l < r)
    {
        int mid = l + (r - l) / 2; 
        sort(arr, l, mid);
        sort(arr, mid + 1, r);
        merge(arr, l, mid, r);
    }
}

int search(int *arr, int x, int left, int right)
{
    int res = -1, m;
    while (left <= right)
    {
        m = (left + right) / 2;
        if (arr[m] == x)
        {
            res = m;
            return 1;
        }
        else if (arr[m] < x)
            left = m + 1;
        else
            right = m - 1;
    }

    return 0;
}


double avg(double *arr)
{
    double ret = 0;
    for(int i = 1; i <= 5; i++)
        ret += arr[i];
    
    ret /= 5;

    return ret;
}
