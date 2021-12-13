#include "heap.h"
#include "insert.h"
#include "quick.h"
#include "set.h"
#include "shell.h"
#include "stats.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//String to be looped through when checking entered arguments
#define OPTIONS "haeisqn:p:r:"

int main(int argc, char **argv) {
    //Initialize default values in beginning, anticipating user does not define them
    int opt = 0;
    Set set = 0;
    int length = 100;
    int seed = 13371453;
    int print = length;
    uint32_t *arr;
    Stats s = { 0, 0 };
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': set = insert_set('h', set); break;
        case 'a': set = insert_set('a', set); break;
        case 'e': set = insert_set('e', set); break;
        case 'i': set = insert_set('i', set); break;
        case 's': set = insert_set('s', set); break;
        case 'q': set = insert_set('q', set); break;
        //Idea to use strtoul and optarg from Eugene's in person section 10/14/21
        case 'n': length = strtoul(optarg, NULL, 10); break;
        case 'p': print = strtoul(optarg, NULL, 10); break;
        case 'r': seed = strtoul(optarg, NULL, 10); break;
        default: insert_set('h', set); break;
        }
    }
    //memory allocation before calling sorting functions
    arr = (uint32_t *) malloc(length * sizeof(uint32_t));

    if (member_set('h', set)) {
        printf(
            "SYNOPSIS \n   A collection of comparison-based sorting algorithms.\n\nUSAGE\n   "
            "./sorting [-haeisqn:p:r:] [-n length] [-p elements] [-r seed]\n\nOPTIONS\n  -h     "
            "         display program help and usage.\n  -a              enable all "
            "sorts.\n  -e              enable Heap Sort.\n  -i              enable Insertion "
            "Sort.\n  -s              enable Shell Sort.\n  -q              enable Quick "
            "Sort.\n  -n length       specify number of elements (default: 100).\n  -p elements  "
            "   specify number of elements to print ( default: 100).\n  -r seed         specify "
            "random seed (default: 13371453)    .\n");
    }

    //array is reset by recalling srandom(SEED) before every sort.
    if (member_set('e', set)) {
        srandom(seed);
        for (int i = 0; i <= length - 1; i++) {
            arr[i] = random() & 0x3FFFFFFF;
        }
        heap_sort(&s, arr, length);
        printf("Heap Sort, %d elements, %" PRIu64 " moves, %" PRIu64 " compares\n", length, s.moves,
            s.compares);
        for (int i = 0; i <= print - 1; i++) {
            printf("%13" PRIu32, arr[i]);
            if ((i + 1) % 5 == 0) {
                printf("\n");
            }
        }
    }

    if (member_set('i', set)) {
        srandom(seed);
        for (int i = 0; i <= length - 1; i++) {
            arr[i] = random() & 0x3FFFFFFF;
        }
        insertion_sort(&s, arr, length);
        printf("Insertion Sort, %d elements, %" PRIu64 " moves, %" PRIu64 " compares\n", length,
            s.moves, s.compares);
        for (int i = 0; i <= print - 1; i++) {
            printf("%13" PRIu32, arr[i]);
            if ((i + 1) % 5 == 0) {
                printf("\n");
            }
        }
    }

    if (member_set('s', set)) {
        srandom(seed);
        for (int i = 0; i <= length - 1; i++) {
            arr[i] = random() & 0x3FFFFFFF;
        }
        shell_sort(&s, arr, length);
        printf("Insertion Sort, %d elements, %" PRIu64 " moves, %" PRIu64 " compares\n", length,
            s.moves, s.compares);
        for (int i = 0; i <= print - 1; i++) {
            printf("%13" PRIu32, arr[i]);
            if ((i + 1) % 5 == 0) {
                printf("\n");
            }
        }
    }
    if (member_set('q', set)) {
        srandom(seed);
        for (int i = 0; i <= length - 1; i++) {
            arr[i] = random() & 0x3FFFFFFF;
        }
        quick_sort(&s, arr, length);
        printf("Insertion Sort, %d elements, %" PRIu64 " moves, %" PRIu64 " compares\n", length,
            s.moves, s.compares);
        for (int i = 0; i <= print - 1; i++) {
            printf("%13" PRIu32, arr[i]);
            if ((i + 1) % 5 == 0) {
                printf("\n");
            }
        }
    }
    if (member_set('a', set)) {
        srandom(seed);
        for (int i = 0; i <= length - 1; i++) {
            arr[i] = random() & 0x3FFFFFFF;
        }
        heap_sort(&s, arr, length);
        printf("Heap Sort, %d elements, %" PRIu64 " moves, %" PRIu64 " compares\n", length, s.moves,
            s.compares);
        for (int i = 0; i <= print - 1; i++) {
            printf("%13" PRIu32, arr[i]);
            if ((i + 1) % 5 == 0) {
                printf("\n");
            }
        }

        srandom(seed);
        for (int i = 0; i <= length - 1; i++) {
            arr[i] = random() & 0x3FFFFFFF;
        }
        shell_sort(&s, arr, length);
        printf("Shell Sort, %d elements, %" PRIu64 " moves, %" PRIu64 " compares\n", length,
            s.moves, s.compares);
        for (int i = 0; i <= print - 1; i++) {
            printf("%13" PRIu32, arr[i]);
            if ((i + 1) % 5 == 0) {
                printf("\n");
            }
        }

        srandom(seed);
        for (int i = 0; i <= length - 1; i++) {
            arr[i] = random() & 0x3FFFFFFF;
        }
        insertion_sort(&s, arr, length);
        printf("Insertion Sort, %d elements, %" PRIu64 " moves, %" PRIu64 " compares\n", length,
            s.moves, s.compares);
        for (int i = 0; i <= print - 1; i++) {
            printf("%13" PRIu32, arr[i]);
            if ((i + 1) % 5 == 0) {
                printf("\n");
            }
        }

        srandom(seed);
        for (int i = 0; i <= length - 1; i++) {
            arr[i] = random() & 0x3FFFFFFF;
        }

        quick_sort(&s, arr, length);
        printf("Quick Sort, %d elements, %" PRIu64 " moves, %" PRIu64 " compares\n", length,
            s.moves, s.compares);
        for (int i = 0; i <= print - 1; i++) {
            printf("%13" PRIu32, arr[i]);
            if ((i + 1) % 5 == 0) {
                printf("\n");
            }
        }
    }

    //checking if no inputs whatsoever entered
    if (union_set(set, empty_set()) == empty_set()) {
        printf("Select at least one sort to perform.\n");
        printf(
            "SYNOPSIS \n   A collection of comparison-based sorting algorithms.\n\nUSAGE\n   "
            "./sorting [-haeisqn:p:r:] [-n length] [-p elements] [-r seed]\n\nOPTIONS\n  -h     "
            "         display program help and usage.\n  -a              enable all "
            "sorts.\n  -e              enable Heap Sort.\n  -i              enable Insertion "
            "Sort.\n  -s              enable Shell Sort.\n  -q              enable Quick "
            "Sort.\n  -n length       specify number of elements (default: 100).\n  -p elements  "
            "   specify number of elements to print ( default: 100).\n  -r seed         specify "
            "random seed (default: 13371453)    .\n");
    }

    free(arr);
}
