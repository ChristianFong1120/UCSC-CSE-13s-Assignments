#include "stats.h"

void insertion_sort(Stats *stats, uint32_t *A, uint32_t n) {
    reset(stats);
    for (uint32_t i = 1; i < n; i++) {
        int j = i;
        uint32_t temp = move(stats, A[i]);
        // if array at current index value is less than value at previous index, move it to previous index
        while (j > 0 && cmp(stats, temp, A[j - 1]) == -1) {
            A[j] = move(stats, A[j - 1]);
            j--;
        }
        //use recently moved down value as temp val
        A[j] = move(stats, temp);
    }
}
