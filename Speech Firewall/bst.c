#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdio.h>
#include "node.h"
uint64_t branches = 0;
Node *bst_create(void) {
    return NULL;
}

//Postorder Traversal
void bst_delete(Node **root) {
    if ((*root) == NULL) {
        return;
    }
    bst_delete(&((*root)->left));
    bst_delete(&((*root)->right));
    node_delete(root);
    free(*root);
    *root = NULL;
}

//Returns height of bst
uint32_t bst_height(Node *root) {
    if (root == NULL) {
        return 0;
    }
    uint32_t left = bst_height(root->left) + 1;
    uint32_t right = bst_height(root->right) + 1;
    if (left > right) {
        return left;
    }
    return right;
}

//Returns size of bst
uint32_t bst_size(Node *root) {
    if (root == NULL) {
        return 0;
    }
    uint32_t left = bst_size(root->left);
    uint32_t right = bst_size(root->right);
    return left + right + 1;
}
//Structure from lecture 18: bst
//Compares ascii values character by character
//and traverses the tree accordingly
Node *bst_find(Node *root, char *oldspeak) {
    if (root != NULL) {
        if (strcmp(root->oldspeak, oldspeak) > 0) {
            branches++;
            return bst_find(root->left, oldspeak);
        } else if (strcmp(root->oldspeak, oldspeak) < 0) {
            branches++;
            return bst_find(root->right, oldspeak);
        }
    }
    return root;
}
//Structure from lecture 18: bst
//Inserts a value into a bst by comparing ascii
//Values character by character and traverses accordingly
Node *bst_insert(Node *root, char *oldspeak, char *newspeak) {
    if (root != NULL) {
        if (strcmp(root->oldspeak, oldspeak) > 0) {
            branches++;
            root->left = bst_insert(root->left, oldspeak, newspeak);
        } else if (strcmp(root->oldspeak, oldspeak) < 0) {
            branches++;
            root->right = bst_insert(root->right, oldspeak, newspeak);
        }
        return root;
    }
    return node_create(oldspeak, newspeak);
}

//Inorder print traversal for alphabetical output
void bst_print(Node *root) {
    if (root == NULL) {
        return;
    }
    bst_print(root->left);
    node_print(root);
    bst_print(root->right);
}
