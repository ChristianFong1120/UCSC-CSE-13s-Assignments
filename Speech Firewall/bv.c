#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include "bv.h"

struct BitVector {
    uint32_t length;
    uint8_t *vector;
};
//Creation of bit vector, allocates memory for array of 8 bit unsigned ints
BitVector *bv_create(uint32_t length) {
    BitVector *bv = (BitVector *) malloc(sizeof(BitVector));
    bv->vector = (uint8_t *) calloc(length, sizeof(uint8_t));
    bv->length = length;
    return bv;
}

//Deletes bit vector
void bv_delete(BitVector **bv) {
    if (*bv && (*bv)->vector) {
        free((*bv)->vector);
        free(*bv);
        *bv = NULL;
    }
}

//Returns bit vector length
uint32_t bv_length(BitVector *bv) {
    return bv->length;
}

//Code structure from code comments repo
//Sets bit if index passed is within range
bool bv_set_bit(BitVector *bv, uint32_t i) {
    if (i <= bv->length) {
        bv->vector[i / 8] |= (0x1 << i % 8);
        return true;
    } else {
        return false;
    }
}
//Code structure from code comments repo
//Clears bit if index passed is within range
bool bv_clr_bit(BitVector *bv, uint32_t i) {
    if (i <= bv_length(bv)) {
        bv->vector[i / 8] &= ~(0x1 << i % 8);
        return true;
    } else {
        return false;
    }
}
//Code structure from code comments repo
//Returns bit if index passed is within range
bool bv_get_bit(BitVector *bv, uint32_t i) {
    if (i <= bv_length(bv)) {

        return (bv->vector[i / 8] >> i % 8) & 0x1;
    }
    return false;
}

//Print bit vector
void bv_print(BitVector *bv) {
    for (uint32_t i = 0; i <= bv_length(bv) * 8; i++) {
        if (bv_get_bit(bv, i)) {
            printf("1");
        } else {
            printf("0");
        }
    }

    printf("\n");
}
