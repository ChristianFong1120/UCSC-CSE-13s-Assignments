#include "stats.h"
//use of math lib for use of log and pow
#include <math.h>
void shell_sort(Stats *stats, uint32_t *A, uint32_t n) {
    uint32_t gap = 0;
    reset(stats);
    uint32_t k = 0;
    uint32_t temp = 0;

    //calculate max val for power of k in gap calculation
    for (uint32_t i = (log(3 + 2 * n)) / log(3); i >= 1; i--) {
        gap = (((pow(3, i)) - 1) / 2);
        for (uint32_t j = gap; j < n; j++) {
            k = j;
            temp = A[j];
            while (k >= gap && cmp(stats, temp, A[k - gap]) == -1) {
                A[k] = move(stats, A[k - i]);
                k -= i;
            }
            A[k] = temp;
        }
    }
}
