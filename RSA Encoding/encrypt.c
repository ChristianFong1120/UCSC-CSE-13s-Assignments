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
#define OPTIONS "n:o:vi:h"

int main(int argc, char **argv) {
    int opt = 0;
    bool help = false;
    bool verbose = false;
    char *pub_file = "rsa.pub";
    char *input_file;
    char *out_file;
    FILE *pub;
    FILE *in = stdin;
    FILE *out = stdout;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': help = true; break;
        case 'n': pub_file = optarg; break;
        case 'o':
            out_file = optarg;
            out = fopen(out_file, "w");
            break;

        case 'i':
            input_file = optarg;
            in = fopen(input_file, "r");
            break;
        case 'v': verbose = true; break;

        default: help = true; break;
        }
    }
    if (help) {
        printf(
            "SYNOPSIS\n    Encrypts data using RSA enryption\n    Encrypted data is decrypted by "
            "the decrypt program.\n\nUSAGE\n    ./encrypt "
            "[-hv] [-i infile] [-o outfile] -n pubkey\n\nOPTIONS\n    -h             Display "
            "program "
            "help and usage.\n    -v             Display verbose program output.\n    -i infile   "
            "   Input file of data to encrypt (default: stdin).\n    -o outfile     "
            "Output file for encrypted data (default: stdout).\n    -n pbfile      "
            "Public key file (default: rsa.pub).\n");
        return 0;
    }
    //Open public file, if null exit program"
    pub = fopen(pub_file, "r");
    if (pub == NULL) {
        printf("Error: invalid public key file\n");
        return 0;
    }

    mpz_t n;
    mpz_t e;
    mpz_t s;
    mpz_t usermpz;
    mpz_inits(n, e, s, usermpz, NULL);

    //Read public keys and username
    char user[256];
    rsa_read_pub(n, e, s, user, pub);

    //If verbose, print keys and bit sizes
    if (verbose) {
        printf("user = %s\n", user);
        gmp_printf("s (%" PRIu64 " bits) = %Zd\nn (%" PRIu64 " bits) = %Zd\ne (%" PRIu64
                   " bits) = %Zd\n",
            mpz_sizeinbase(s, 2), s, mpz_sizeinbase(n, 2), n, mpz_sizeinbase(e, 2), e);
    }

    //If invalid signature, exit
    mpz_set_str(usermpz, user, 62);
    if (!rsa_verify(usermpz, s, e, n)) {
        printf("Error: invalid signature\n");
        return 0;
    }
    rsa_encrypt_file(in, out, n, e);

    fclose(in);
    fclose(pub);
    fclose(out);
    mpz_clears(n, e, s, usermpz, NULL);
    return 0;
}
