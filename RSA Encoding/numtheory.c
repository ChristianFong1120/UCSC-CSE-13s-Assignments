#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <gmp.h>
#include "randstate.h"
#include "numtheory.h"
#include <time.h>

//Performs modular exponentiation
void pow_mod(mpz_t out, mpz_t base, mpz_t exponent, mpz_t modulus) {
    //initialize mpz vars
    mpz_t v;
    mpz_t p;
    mpz_t d;
    mpz_t d_mod_2;
    mpz_inits(v, p, d, d_mod_2, NULL);
    mpz_set_ui(v, 1);
    mpz_set(p, base);
    mpz_set(d, exponent);
    while (mpz_cmp_ui(d, 0) > 0) {
        mpz_mod_ui(d_mod_2, d, 2);
        if (mpz_cmp_ui(d_mod_2, 0) != 0) {
            mpz_mul(v, v, p);
            mpz_mod(v, v, modulus);
        }
        mpz_mul(p, p, p);
        mpz_mod(p, p, modulus);
        mpz_fdiv_q_ui(d, d, 2);
    }
    mpz_set(out, v);
    mpz_clears(v, p, d, d_mod_2, NULL);
}

//Uses Miller-Rabin test of primality
bool is_prime(mpz_t n, uint64_t iters) {
    //2 is the only even prime number, 3 returned false, as we need to measure a range of 2 to n - 1 later
    //In case of 3, 3 - 1 = 2, so a range[ 2 , 2] would be yielded
    if (mpz_cmp_ui(n, 2) == 0 || mpz_cmp_ui(n, 3) == 0) {
        return true;
    }
    mpz_t is_even;
    mpz_init(is_even);
    mpz_mod_ui(is_even, n, 2);
    //If even, immediately return false
    if (mpz_cmp_ui(is_even, 0) == 0 || mpz_cmp_ui(n, 1) == 0) {
        return false;
    }

    mpz_t a;

    mpz_t r;
    mpz_t n_minus_one;
    mpz_t s_minus_one;
    uint32_t s = 0;
    mpz_t y;
    mpz_t j;
    mpz_t range;
    mpz_t two;
    mpz_init_set_ui(two, 2);
    mpz_t r_mod_2;
    mpz_inits(y, j, a, r, range, r_mod_2, n_minus_one, s_minus_one, NULL);
    mpz_sub_ui(n_minus_one, n, 1);
    mpz_t n_minus_two;
    mpz_init(n_minus_two);
    mpz_sub_ui(n_minus_two, n, 2);
    mpz_sub_ui(r, n, 1);
    mpz_sub_ui(range, n, 2);
    mpz_set_ui(a, 0);
    mpz_mod_ui(r_mod_2, r, 2);
    //Finding value for s
    while (mpz_cmp_ui(r_mod_2, 0) == 0) {
        mpz_fdiv_q_ui(r, r, 2);
        mpz_mod_ui(r_mod_2, r, 2);
        s++;
    }
    s -= 1;
    mpz_set_ui(s_minus_one, s);
    for (uint64_t i = 0; i <= iters; i++) {
        mpz_sub_ui(a, a, 2);
        pow_mod(y, a, r, n);
        if (mpz_cmp(y, n_minus_one) == 0) {
            pow_mod(y, a, r, n);
        }
        if (mpz_cmp_ui(y, 1) != 0 && mpz_cmp(y, n_minus_one) != 0) {
            mpz_set_ui(j, 1);
            while (((mpz_cmp(j, s_minus_one) <= 0) && mpz_cmp(y, n_minus_one) != 0)) {
                pow_mod(y, y, two, n);
                if (mpz_cmp_ui(y, 1) == 0) {

                    mpz_clears(is_even, a, r, n_minus_one, s_minus_one, y, j, range, two, r_mod_2,
                        n_minus_two, NULL);
                    return false;
                }
                mpz_add_ui(j, j, 1);
            }
            if (mpz_cmp(y, n_minus_one) != 0) {
                mpz_clears(is_even, a, r, n_minus_one, s_minus_one, y, j, range, two, r_mod_2,
                    n_minus_two, NULL);
                return false;
            }
        }
    }
    mpz_clears(
        is_even, a, r, n_minus_one, s_minus_one, y, j, range, two, r_mod_2, n_minus_two, NULL);
    return true;
}

//Creates random prime number of bit size bits
void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {
    mpz_t prime;
    mpz_init_set_ui(prime, 10);
    //Generate another random if its not prime / or less than specified bit size
    while (!(is_prime(prime, iters) && mpz_sizeinbase(prime, 2) == bits + 1)) {
        mpz_urandomb(prime, state, bits + 1);
    }
    mpz_set(p, prime);
    mpz_clear(prime);
}

//Performs modular inverse
void mod_inverse(mpz_t i, mpz_t a, mpz_t n) {
    mpz_t r;
    mpz_init_set(r, n);
    mpz_t r2;
    mpz_init_set(r2, a);
    mpz_t t;
    mpz_init_set_ui(t, 0);
    mpz_t t2;
    mpz_init_set_ui(t2, 1);
    mpz_t q;
    mpz_t temp;
    mpz_t r_div_r2;
    mpz_t calc;

    mpz_inits(q, calc, temp, r_div_r2, NULL);
    while (mpz_cmp_ui(r2, 0) != 0) {
        mpz_fdiv_q(r_div_r2, r, r2);

        mpz_set(q, r_div_r2);
        mpz_mul(q, q, r2);
        mpz_set(temp, r);
        mpz_set(r, r2);
        mpz_sub(temp, temp, q);
        mpz_set(r2, temp);
        mpz_set(q, r_div_r2);
        mpz_set(temp, t);
        mpz_set(t, t2);
        mpz_mul(q, q, t2);
        mpz_sub(temp, temp, q);

        mpz_set(t2, temp);
    }
    if (mpz_cmp_ui(r, 1) > 0) {
        mpz_set_ui(t, 0);
    }
    if (mpz_cmp_ui(t, 0) < 0) {
        mpz_add(t, t, n);
    }
    mpz_set(i, t);
    mpz_clears(r, r2, t, t2, q, temp, calc, r_div_r2, NULL);
}

//Computes greatest common divisor
void gcd(mpz_t d, mpz_t a, mpz_t b) {
    mpz_t t;
    mpz_t b_copy;
    mpz_t a_mod_b;
    mpz_t a_copy;
    mpz_inits(t, a_mod_b, a_copy, b_copy, NULL);
    mpz_set(a_copy, a);
    mpz_set(b_copy, b);
    while (mpz_cmp_ui(b_copy, 0) != 0) {
        mpz_set(t, b_copy);
        mpz_mod(a_mod_b, a_copy, b_copy);
        mpz_set(b_copy, a_mod_b);
        mpz_set(a_copy, t);
    }
    mpz_set(d, a_copy);
    mpz_clear(t);
    mpz_clear(a_mod_b);
    mpz_clear(b_copy);
    mpz_clear(a_copy);
}
