#include <inttypes.h>
#include <stdlib.h>

#include <stdio.h>
#include "bf.h"
#include "speck.h"
#include "bv.h"
#include "salts.h"

struct BloomFilter {
    uint64_t primary[2]; // Primary hash function salt.
    uint64_t secondary[2]; // Secondary hash function salt.
    uint64_t tertiary[2]; // Tertiary hash function salt.
    BitVector *filter;
};

//Creation of bloom filter
BloomFilter *bf_create(uint32_t size) {
    BloomFilter *bf = (BloomFilter *) malloc(sizeof(BloomFilter));
    bf->filter = bv_create(size);
    bf->primary[0] = SALT_PRIMARY_LO;
    bf->primary[1] = SALT_PRIMARY_HI;
    bf->secondary[0] = SALT_SECONDARY_LO;
    bf->secondary[1] = SALT_SECONDARY_HI;
    bf->tertiary[0] = SALT_TERTIARY_LO;
    bf->tertiary[1] = SALT_TERTIARY_HI;
    return bf;
}

//Delete bit vector as well as bloom filter struct
void bf_delete(BloomFilter **bf) {
    if (*bf && (*bf)->filter) {
        bv_delete(&(*bf)->filter);
        free(*bf);
        *bf = NULL;
    }
}

//Returns size of bloom filter
uint32_t bf_size(BloomFilter *bf) {
    return bv_length(bf->filter);
}

//Sets bits at the three indicies returned by hashing the oldspeak
void bf_insert(BloomFilter *bf, char *oldspeak) {
    uint32_t index;
    index = hash(bf->primary, oldspeak) % bf_size(bf);
    bv_set_bit(bf->filter, index);
    index = hash(bf->secondary, oldspeak) % bf_size(bf);
    bv_set_bit(bf->filter, index);
    index = hash(bf->tertiary, oldspeak) % bf_size(bf);
    bv_set_bit(bf->filter, index);
}

//Checks for membership of a value in a bloomfilter
//by hashing and checking the three returned indices
bool bf_probe(BloomFilter *bf, char *oldspeak) {
    uint32_t index1 = hash(bf->primary, oldspeak) % bf_size(bf);
    uint32_t index2 = hash(bf->secondary, oldspeak) % bf_size(bf);
    uint32_t index3 = hash(bf->tertiary, oldspeak) % bf_size(bf);
    if (bv_get_bit(bf->filter, index1) && bv_get_bit(bf->filter, index2)
        && bv_get_bit(bf->filter, index3)) {
        return true;
    } else {
        return false;
    }
}

//Returns the number of set bits in bloom filter
uint32_t bf_count(BloomFilter *bf) {
    uint32_t c = 0;
    for (uint32_t i = 0; i <= bv_length(bf->filter); i++) {
        if (bv_get_bit(bf->filter, i)) {
            c++;
        }
    }
    return c;
}
//Print bloom filter
void bf_print(BloomFilter *bf) {
    bv_print(bf->filter);
}
