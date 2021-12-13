#include "stats.h"

//general structure of all functions courtesy of assignment document under Quicksort

int partition(Stats *stats, uint32_t *A, int low, int high) {
    int i = low - 1;
    for (int k = low; k < high; k++) {
        //if first val of A is less than last val of A, increase iterator
        if (cmp(stats, A[k - 1], A[high - 1]) == -1) {
            i += 1;
            swap(stats, &A[k - 1], &A[i - 1]);
        }
    }
    swap(stats, &A[i], &A[high - 1]);
    return i + 1;
}

//Function recursively calls itself, increasing its high and low vals in converging fashion
//and partitions them each call
void sorter(Stats *stats, uint32_t *A, int low, int high) {
    if (low < high) {
        int p = partition(stats, A, low, high);
        sorter(stats, A, low, p - 1);
        sorter(stats, A, p + 1, high);
    }
}

void quick_sort(Stats *stats, uint32_t *A, uint32_t n) {
    //    reset(stats);
    sorter(stats, A, 1, n);
}
