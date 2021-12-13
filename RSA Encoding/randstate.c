#include <gmp.h>
#include <inttypes.h>
#include "randstate.h"

gmp_randstate_t state;
//Initialize randstate with passed seed
void randstate_init(uint64_t seed) {
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, seed);
}
//Clear state
void randstate_clear(void) {
    gmp_randclear(state);
}
