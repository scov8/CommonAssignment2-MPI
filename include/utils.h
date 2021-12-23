#ifndef H_UTILS
#define H_UTILS

int getMax(int *arr, int n);

void generateArray(int *arr, int dim);

void generateArrayParallel(int arr[], int n, int rank, int nprocs);

#endif
