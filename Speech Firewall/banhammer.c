#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <inttypes.h>
#include <regex.h>
#include <math.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include "speck.h"
#include "bv.h"
#include "bst.h"
#include "ht.h"
#include "node.h"
#include "parser.h"
#include "salts.h"
#include "messages.h"
#include "bf.c"
#define OPTIONS "ht:f:s"
#define WORD    "[a-zA-Z0-9]+(('|-)[a-zA-Z0-9]+)*"

int main(int argc, char **argv) {
    int opt = 0;
    bool help = false;
    uint32_t hash_size = pow(2, 16);
    uint32_t bloom_size = pow(2, 20);
    bool stats = false;
    //Read CLI
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': help = true; break;
        case 't': hash_size = strtoul(optarg, NULL, 10); break;
        case 'f': bloom_size = strtoul(optarg, NULL, 10); break;
        case 's': stats = true; break;
        }
    }

    //Print help message and exit
    if (help) {
        printf("SYNOPSIS\n  A word filtering program for the GPRSC.\n  Filters out and reports bad "
               "words parsed from stdin.\nUSAGE\n  ./banhammer [-hs] [-t size] [-f "
               "size]\nOPTIONS\n  -h           Program usage and help.\n  -s           Print "
               "program statistics.\n  -t size      Specify hash table size (default: 10000).\n  "
               "-f size      Specify Bloom filter size (default: 2^20).\n");
        return 1;
    }
    //Create hash table and bloom filter of specified sizes
    BloomFilter *bf = bf_create(bloom_size);
    HashTable *ht = ht_create(hash_size);

    //Create file descriptors
    FILE *badfile = fopen("badspeak.txt", "r");
    FILE *newfile = fopen("newspeak.txt", "r");
    //Read in oldspeak
    char bad[1024];
    while (fscanf(badfile, "%s\n", bad) != EOF) {
        bf_insert(bf, bad);
        ht_insert(ht, bad, NULL);
    }

    //Read in oldspeak with newspeak translations
    char new[1024];
    char old[1024];
    while ((fscanf(newfile, "%s %s\n", old, new) != EOF)) {
        bf_insert(bf, old);
        ht_insert(ht, old, new);
    }

    //Check if valid regex
    //Provided by assignment documentation
    regex_t re;
    if (regcomp(&re, WORD, REG_EXTENDED)) {
        fprintf(stderr, "Failed to compile regex.\n");
        return 1;
    }

    //To store current parsed word
    char *word = NULL;

    //To store user's bad words and words that will need counseling
    Node *badwords = bst_create();
    Node *goodwords = bst_create();

    //Bools for deciding the punishment/counseling of a citizen
    bool thought = false;
    bool counseling = false;

    //stdin parser, provided by assignment documentation
    while ((word = next_word(stdin, &re)) != NULL) {
        //Account for capitalizations
        for (uint8_t i = 0; i <= strlen(word); i++) {
            word[i] = tolower(word[i]);
        }
        //Is word in bloom filter?
        //If so, also check hash table
        if (bf_probe(bf, word)) {
            //Node to check if there is newspeak translation
            Node *checker = ht_lookup(ht, word);
            if (checker != NULL) {
                //Checking if word passed has a newspeak translation
                if (checker->newspeak != NULL) {
                    counseling = true;
                    goodwords = bst_insert(goodwords, checker->oldspeak, checker->newspeak);
                }
                //Checking if word passed does not have a translation and is just a bad word
                else if (checker->newspeak == NULL) {
                    thought = true;
                    badwords = bst_insert(badwords, checker->oldspeak, NULL);
                }
            }
        }
    }
    //Statsitical print
    if (stats) {
        printf("Average BST size: %.6f\n", ht_avg_bst_size(ht));
        printf("Average BST height: %.6f\n", ht_avg_bst_height(ht));
        printf("Average branches traversed: %.6f\n", ((double) branches / lookups));
        printf("Hash table load: %.6f%%\n", (100.0 * ((double) (ht_count(ht)) / ht_size(ht))));
        printf("Bloom filter load: %.6f%%\n", (100.0 * ((double) (bf_count(bf)) / bf_size(bf))));
        return 1;
    }

    //Based on booleans set, deciding punsihment for citizen
    if (thought && !(counseling)) {
        printf("%s", badspeak_message);
        bst_print(badwords);
    } else if (counseling && !(thought)) {
        printf("%s", goodspeak_message);
        bst_print(goodwords);
    } else if (counseling && thought) {
        printf("%s", mixspeak_message);
        bst_print(badwords);
        bst_print(goodwords);
    }

    //Closing files and deleting structs
    fclose(badfile);
    fclose(newfile);
    clear_words();
    regfree(&re);
    bf_delete(&bf);
    ht_delete(&ht);
    bst_delete(&badwords);
    bst_delete(&goodwords);
}
