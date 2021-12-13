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
    char *priv_file = "rsa.priv";
    char *input_file;
    char *out_file;
    FILE *priv;
    FILE *in = stdin;
    FILE *out = stdout;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': help = true; break;
        case 'n': priv_file = optarg; break;
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
            "SYNOPSIS\n    Decrypts data using RSA decryption\n    Encrypted data is encrypted by "
            "the encrypt program.\n\nUSAGE\n    ./decrypt "
            "[-hv] [-i infile] [-o outfile] -n privkey\n\nOPTIONS\n    -h             Display "
            "program "
            "help and usage.\n    -v             Display verbose program output.\n    -i infile   "
            "   Input file of data to decrypt (default: stdin).\n    -o outfile  "
            "   Output file for decrypted data (default: stdout).\n    -n pvfile     "
            " Private key file (default: rsa.priv).\n");
        return 0;
    }
    //If private key file fails to open, exit
    priv = fopen(priv_file, "r");
    if (priv == NULL) {
        printf("Error: invalid private key file\n");
        return 0;
    }
    //Read in private keys
    mpz_t n;
    mpz_t d;
    mpz_inits(n, d, NULL);
    rsa_read_priv(n, d, priv);

    //If verbose, print private key values and bit sizes
    if (verbose) {
        gmp_printf("n (%" PRIu64 " bits) = %Zd\nd (%" PRIu64 " bits) = %Zd\n", mpz_sizeinbase(n, 2),
            n, mpz_sizeinbase(d, 2), d);
    }

    //Decrypt file
    rsa_decrypt_file(in, out, n, d);

    fclose(priv);
    fclose(in);
    fclose(out);
    mpz_clears(n, d, NULL);
}
