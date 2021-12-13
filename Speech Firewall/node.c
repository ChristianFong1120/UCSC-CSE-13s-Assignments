#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "node.h"

//Creation of node, allocate memory for oldspeak
//and newspeak if it exists
Node *node_create(char *old, char *new) {
    Node *n = (Node *) calloc(1, sizeof(Node));
    n->oldspeak = (char *) malloc(sizeof(char));
    n->oldspeak = strdup(old);
    //If passed node to create has a newspeak
    if (new) {
        n->newspeak = (char *) malloc(sizeof(char));
        n->newspeak = strdup(new);
    }
    n->left = NULL;
    n->right = NULL;
    return n;
}

//Deleting a node and freeing oldspeak and newspeak memory
void node_delete(Node **n) {
    if (*n) {
        free((*n)->oldspeak);
        free((*n)->newspeak);
        free(*n);
        *n = NULL;
    }
}
//Print the node oldspeak, newspeak if it exists
void node_print(Node *n) {
    if (n->newspeak != NULL) {
        printf("%s -> %s\n", n->oldspeak, n->newspeak);
    } else {
        printf("%s\n", n->oldspeak);
    }
}
