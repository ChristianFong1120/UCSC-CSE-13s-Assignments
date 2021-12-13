#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include "ht.h"
#include "speck.h"
#include "bst.h"
#include "node.h"
#include "salts.h"
uint64_t lookups = 0;

struct HashTable {
    uint64_t salt[2];
    uint32_t size;
    Node **trees;
};

//Creation of hash table
HashTable *ht_create(uint32_t size) {
    HashTable *ht = (HashTable *) malloc(sizeof(HashTable));
    ht->salt[0] = SALT_HASHTABLE_LO;
    ht->salt[1] = SALT_HASHTABLE_HI;
    ht->size = size;
    ht->trees = (Node **) calloc(size, sizeof(Node *));
    return ht;
}
//Delete hash table, check each non null bst in table
//And delete it, as well as the array and struct itself
void ht_delete(HashTable **ht) {
    if (*ht) {
        for (uint32_t i = 0; i <= ht_size(*ht) - 1; i++) {
            Node *checker = (*ht)->trees[i];
            if (checker) {
                bst_delete(&(checker));
            }
        }
        free((*ht)->trees);
        free(*ht);
        *ht = NULL;
    }
}

//Returns Size
uint32_t ht_size(HashTable *ht) {
    return ht->size;
}

//Returns root of where the sought string is stored
Node *ht_lookup(HashTable *ht, char *oldspeak) {
    lookups++;
    uint32_t index = hash(ht->salt, oldspeak) % ht->size;
    if (ht->trees[index] != NULL) {
        return bst_find(ht->trees[index], oldspeak);
    }
    return NULL;
}

//Inserts a value into bst within the hash table
void ht_insert(HashTable *ht, char *oldspeak, char *newspeak) {
    lookups++;
    uint32_t index = hash(ht->salt, oldspeak) % ht->size;
    ht->trees[index] = bst_insert(ht->trees[index], oldspeak, newspeak);
}

//Returns count of non null trees
uint32_t ht_count(HashTable *ht) {
    uint32_t c = 0;
    for (uint32_t i = 0; i <= ht->size - 1; i++) {
        if (ht->trees[i] != NULL) {
            c++;
        }
    }
    return c;
}

//Returns average bst size of hash table
double ht_avg_bst_size(HashTable *ht) {
    double sizes = 0;
    for (uint32_t i = 0; i <= ht->size - 1; i++) {
        if (ht->trees[i] != NULL) {
            sizes += bst_size(ht->trees[i]);
        }
    }
    double avg = sizes / ht_count(ht);
    return avg;
}

//Returns average bst height of hash table
double ht_avg_bst_height(HashTable *ht) {
    double heights = 0;
    for (uint32_t i = 0; i <= ht->size - 1; i++) {
        if (ht->trees[i] != NULL) {
            heights += bst_height(ht->trees[i]);
        }
    }
    double avg = heights / ht_count(ht);
    return avg;
}
//Print hash table
void ht_print(HashTable *ht) {
    for (uint32_t i = 0; i <= ht->size - 1; i++) {
        printf("i: %" PRIu32 "\n", i);
        if (ht->trees[i] != NULL) {
            bst_print(ht->trees[i]);
        } else {

            printf("NULL\n");
        }
    }
}
