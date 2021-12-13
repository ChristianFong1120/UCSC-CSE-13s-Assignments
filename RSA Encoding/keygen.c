#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "rsa.h"
#include "randstate.h"
#include "numtheory.h"
#include <inttypes.h>
#include <unistd.h>
#include <sys/stat.h>
#define OPTIONS "b:i:n:d:s:vh"

int main(int argc, char **argv) {
    int opt = 0;
    bool help = false;
    uint64_t nbits = 256;
    uint64_t iters = 50;
    uint64_t seed = time(NULL);
    bool verbose = false;
    char *pub_file = "rsa.pub";
    char *priv_file = "rsa.priv";
    FILE *pub;
    FILE *priv;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': help = true; break;
        case 'b': nbits = strtoul(optarg, NULL, 10); break;
        case 'i': iters = strtoul(optarg, NULL, 10); break;
        case 'd': pub_file = optarg; break;
        case 'n': priv_file = optarg; break;
        case 's': seed = strtoul(optarg, NULL, 10); break;
        case 'v': verbose = true; break;
        default: help = true; break;
        }
    }
    if (help) {
        printf("SYNOPSIS\n    Generates an RSA public/private key pair.\n\nUSAGE\n    ./keygen "
               "[-hv] [-b bits] -n pbfile -d pvfile\n\nOPTIONS\n    -h             Display program "
               "help and usage.\n    -v             Display verbose program output.\n    -b bits   "
               "     Minimum bits needed for public key n (default: 256)\n    -i confidence  "
               "Miller-Rabin iterations for testing primes (default: 50)\n    -n pbfile      "
               "Public key file (default: rsa.pub).\n    -d pvfile      Private key file (default: "
               "rsa.priv).\n    -s seed        Random seed for testing.\n");
        return 0;
    }
    //Open public and private key files, if unable to, exit
    pub = fopen(pub_file, "w");
    priv = fopen(priv_file, "w");
    if (pub == NULL || priv == NULL) {
        printf("Error: invalid public / private key file(s).\n");
        return 0;
    }
    //Set permissions of private key file so only user can view/write
    fchmod(fileno(priv), 0600);
    randstate_init(seed);
    mpz_t p;
    mpz_t q;
    mpz_t n;
    mpz_t e;
    mpz_t usermpz;
    mpz_t d;
    mpz_t s;
    mpz_inits(s, usermpz, p, q, n, e, d, NULL);
    mpz_set_str(usermpz, getenv("USER"), 62);

    //Generate key values
    rsa_make_pub(p, q, n, e, nbits, iters);
    rsa_make_priv(d, e, p, q);
    rsa_sign(s, usermpz, d, n);

    //Write key values
    rsa_write_pub(n, e, s, getenv("USER"), pub);
    rsa_write_priv(n, d, priv);

    //If verbose true, print keys and bit sizes
    if (verbose) {
        printf("user: %s\n", getenv("$USER"));
        gmp_printf("s (%" PRIu64 " bits) = %Zd\np (%" PRIu64 " bits) = %Zd\nq (%" PRIu64
                   " bits) = %Zd\nn (%" PRIu64 " bits) = "
                   "%Zd\ne (%" PRIu64 " bits) = %Zd\nd (%" PRIu64 " bits) = %Zd\n",
            mpz_sizeinbase(s, 2), s, mpz_sizeinbase(p, 2), p, mpz_sizeinbase(q, 2), q,
            mpz_sizeinbase(n, 2), n, mpz_sizeinbase(e, 2), e, mpz_sizeinbase(d, 2), 2);
    }
    fclose(pub);
    fclose(priv);
    randstate_clear();
    mpz_clears(s, usermpz, p, q, n, e, d, NULL);
    return 0;
}
