#include "stats.h"

#include <stdbool.h>

//general structure of all functions courtesy of assignment doc under Heapsort

//helper function for checking which child (left or right) is of greater value
uint32_t max_child(Stats *stats, uint32_t *A, uint32_t first, uint32_t last) {
    uint32_t left = first * 2;
    uint32_t right = left + 1;
    if (right <= last && cmp(stats, A[right - 1], A[left - 1]) == 1) {
        return right;
    }
    return left;
}

//helper function to sort array in fashion of a max heap
void fix_heap(Stats *stats, uint32_t *A, uint32_t first, uint32_t last) {
    bool found = false;
    uint32_t parent = first;
    uint32_t largestVal = max_child(stats, A, parent, last);

    while (parent <= (last / 2) && found == false) {
        if (cmp(stats, A[parent - 1], A[largestVal - 1]) == -1) {
            swap(stats, &A[parent - 1], &A[largestVal - 1]);
            parent = largestVal;
            largestVal = max_child(stats, A, parent, last);
        } else {
            found = true;
        }
    }
}

//building the actual heap using previously defined helpers
void build_heap(Stats *stats, uint32_t *A, uint32_t first, uint32_t last) {
    for (uint32_t i = last / 2; i > first - 1; i--) {
        fix_heap(stats, A, i, last);
    }
}

void heap_sort(Stats *stats, uint32_t *A, uint32_t n) {
    uint32_t first = 1;
    uint32_t last = n;
    build_heap(stats, A, first, last);
    for (uint32_t i = last; i > first; i--) {
        //Use of & whenever swap, as we want to actually change the values it points to
        swap(stats, &A[first - 1], &A[i - 1]);
        fix_heap(stats, A, first, i - 1);
    }
}
