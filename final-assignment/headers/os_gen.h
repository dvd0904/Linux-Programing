#ifndef SS_H
#define SS_H

/**
 * @brief Implement merger sort
 * @param arr array to sort
 * @param l first index
 * @param r last index
*/
void sort(int *arr, int l, int r);

/* Binary search */
/**
 * @brief Implement binary search
 * @param arr array search
 * @param x element need to search
 * @param left first index
 * @param right last index
 * @return 1 if found otherwise 0
*/
int search(int *arr, int x, int left, int right);

/**
 * @brief Calculate average of all element an array
 * @param arr an array
 * @return average of an array
*/
double avg(double *arr);


#endif