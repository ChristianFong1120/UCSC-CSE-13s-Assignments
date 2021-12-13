#include "rsa.h"
#include "numtheory.h"
#include "randstate.h"
#include <gmp.h>
#include <inttypes.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//Creates public keys
void rsa_make_pub(mpz_t p, mpz_t q, mpz_t n, mpz_t e, uint64_t nbits, uint64_t iters) {
    srand(time(NULL));
    uint64_t upper_range = (3 * nbits) / 4;
    uint64_t min_range = nbits / 4;
    //Get random number between min_range and upper_range
    uint64_t p_bits = rand() % upper_range + min_range;
    while (p_bits > upper_range) {
        p_bits = rand() % upper_range + min_range;
    }
    uint64_t q_bits = (nbits - p_bits);
    mpz_t p_copy;
    mpz_t q_copy;
    mpz_t rand;
    mpz_t gcd_container;
    mpz_t totient;
    mpz_inits(totient, rand, p_copy, q_copy, NULL);
    //Creation of P and Q
    make_prime(p_copy, p_bits, iters);
    make_prime(q_copy, q_bits, iters);
    mpz_set(p, p_copy);
    mpz_set(q, q_copy);
    mpz_mul(n, p_copy, q_copy);
    mpz_sub_ui(p_copy, p, 1);
    mpz_sub_ui(q_copy, q, 1);
    mpz_init_set_ui(gcd_container, 10);
    mpz_mul(totient, p_copy, q_copy);
    mp_bitcnt_t bits_length = nbits;
    //Continue generating random number until
    //Generated E value is coprime to totient
    while (mpz_cmp_ui(gcd_container, 1) != 0) {
        mpz_urandomb(rand, state, bits_length);
        gcd(gcd_container, rand, totient);
    }
    mpz_set(e, rand);
    mpz_clears(p_copy, q_copy, totient, rand, gcd_container, NULL);
}

//Writes public keys to outfile
void rsa_write_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    gmp_fprintf(pbfile, "%Zx\n%Zx\n%Zx\n", n, e, s);
    fprintf(pbfile, "%s\n", username);
}
//Reads public keys from infile
void rsa_read_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    gmp_fscanf(pbfile, "%Zx\n%Zx\n%Zx\n", n, e, s);
    fscanf(pbfile, "%s\n", username);
}

//Creates private keys
void rsa_make_priv(mpz_t d, mpz_t e, mpz_t p, mpz_t q) {
    mpz_t totient;
    mpz_t p_copy;
    mpz_t q_copy;
    mpz_inits(totient, p_copy, q_copy, NULL);
    mpz_sub_ui(p_copy, p, 1);
    mpz_sub_ui(q_copy, q, 1);
    mpz_mul(totient, p_copy, q_copy);
    mod_inverse(d, e, totient);
    mpz_clears(totient, p_copy, q_copy, NULL);
}

//Writes private keys to outfile
void rsa_write_priv(mpz_t n, mpz_t d, FILE *pvfile) {
    gmp_fprintf(pvfile, "%Zx\n%Zx\n", n, d);
}

//Reads private keys from infile
void rsa_read_priv(mpz_t n, mpz_t d, FILE *pvfile) {
    gmp_fscanf(pvfile, "%Zx\n%Zx\n", n, d);
}

//Calculation for encryption
void rsa_encrypt(mpz_t c, mpz_t m, mpz_t e, mpz_t n) {
    pow_mod(c, m, e, n);
}

//Encrypts a file by reading line of bytes from infile into a buffer
//Buffer is imported to mpz then encrypted and written to outfile in hexstring
void rsa_encrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t e) {
    uint64_t block_size = (log2(mpz_get_d(n)) - 1) / 8;
    uint8_t *buf;
    //Create buffer and clear it
    buf = (uint8_t *) malloc(block_size * sizeof(uint8_t));
    memset(buf, 0, block_size);
    //Prepend buffer
    buf[0] = 0xFF;
    mpz_t write_container;
    mpz_t read_container;
    mpz_inits(read_container, write_container, NULL);
    int j = 1;
    while (!feof(infile)) {
        j = fread(buf + 1, 1, block_size - 1, infile);
        mpz_import(read_container, j + 1, 1, 1, 1, 0, buf);
        rsa_encrypt(write_container, read_container, e, n);
        gmp_fprintf(outfile, "%Zx\n", write_container);
    }
    free(buf);
    mpz_clears(read_container, write_container, NULL);
}

void rsa_decrypt(mpz_t m, mpz_t c, mpz_t d, mpz_t n) {
    pow_mod(m, c, d, n);
}

//Decrypts a file by reading hexstrings from infile into a buffer
//Buffer is then decrypted and exported from mpz to bytes
//Buffer of bytes is then written to outfile
void rsa_decrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t d) {
    uint64_t block_size = (log2(mpz_get_d(n)) - 1) / 8;
    //Create buffer and allocate memory, also clear buffer
    uint8_t *buf;
    buf = (uint8_t *) malloc(block_size * sizeof(uint8_t));
    memset(buf, 0, block_size);
    mpz_t c;
    unsigned long int *j = &block_size;
    mpz_t write_container;
    mpz_inits(write_container, c, NULL);
    while (!feof(infile)) {
        *j = gmp_fscanf(infile, "%Zx\n", c);
        rsa_decrypt(write_container, c, d, n);
        mpz_export(buf, j, 1, 1, 1, 0, write_container);
        fwrite(buf + 1, *j - 1, 1, outfile);
    }
    free(buf);
    mpz_clears(write_container, c, NULL);
}

//Calculation for signature
void rsa_sign(mpz_t s, mpz_t m, mpz_t d, mpz_t n) {
    pow_mod(s, m, d, n);
}

//Verifies a passed signature
bool rsa_verify(mpz_t m, mpz_t s, mpz_t e, mpz_t n) {
    mpz_t t;
    mpz_init(t);
    pow_mod(t, s, e, n);
    if (mpz_cmp(t, m) == 0) {
        mpz_clear(t);
        return true;
    }
    mpz_clear(t);
    return false;
}
