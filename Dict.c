// COMP2521 20T2 Assignment 1
// Dict.c ... implementsation of Dictionary ADT

#include "Dict.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "WFreq.h"

typedef struct _DictNode *Link;

typedef struct _DictNode {
    WFreq data;
    Link left;
    Link right;
    int height;
} DictNode;

struct _DictRep {
    Link tree;
};
//
// define functions
Link Insert(Link n, Link newNode);
void recShow(Link node, int space);
int height(Link n);
int max(int a, int b);
Link rotateLeft(Link n);
Link rotateRight(Link n);
Link Find(Link n, char *w);

// create new empty Dictionary
Dict newDict() {
    // TODO
    Dict d = malloc(sizeof(*d));
    d->tree = NULL;
    return d;
}

// insert new word into Dictionary
// return pointer to the (word,freq) pair for that word
WFreq *DictInsert(Dict d, char *w) {
    // TODO
    WFreq newWord;
    newWord.word = w;
    newWord.freq = 1;
    Link newNode = malloc(sizeof(*newNode));
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->data = newWord;
    newNode->height = 0;

    d->tree = Insert(d->tree, newNode);
    return &newNode->data;
}

Link Insert(Link n, Link newNode) {
    if (n == NULL) {
        return newNode;
    }
    // insert recursively
    int cmp = strcmp(n->data.word, newNode->data.word);
    if (cmp < 0) {
        n->left = Insert(n->left, newNode);
    } else if (cmp > 0) {
        n->right = Insert(n->right, newNode);
    } else {
        return n;
    }

    // insertion done, correct height
    // referenced from lecture slides
    n->height = 1 + max(height(n->left), height(n->right));
    //
    // rebalance the tree
    // referenced from 
    // https://www.cs.swarthmore.edu/~brody/cs35/f14/Labs/extras/08/avl_pseudo.pdf
    int LHeight = height(n->left);
    int RHeight = height(n->right);
    if ((LHeight - RHeight) > 1) {
        if (n->left != NULL) {
            if (height(n->left->left) < height(n->left->right)) {
                n->left = rotateLeft(n->left);
            }
            n = rotateRight(n);
        }
    } else if ((RHeight - LHeight) > 1) {
        if (n->right != NULL) {
            if (height(n->right->left) < height(n->right->right)) {
                n->right = rotateRight(n->right);
            }
            n = rotateLeft(n);
        }
    }
    return n;
}

// find Word in Dictionary
// return pointer to (word,freq) pair, if found
// otherwise return NULL
WFreq *DictFind(Dict d, char *w) {
    // TODO
    if (d == NULL) {
        return NULL;
    }
    Link temp = Find(d->tree, w);
    if (temp == NULL) {
        return NULL; // not found
    } else {
        return &temp->data;
    }
    
}

// recursively find w in binary tree n
// returns node in tree or null
Link Find(Link n, char *w) {
    
    if (n == NULL) {
        return NULL;
    }
    int cmp = strcmp(n->data.word, w);
    if (cmp < 0) {
        return Find(n->left, w);
    } else if (cmp > 0) {
        return Find(n->right, w);
    } else { //found!
        return n;   // do i return n? 
    }
    return NULL;
}

// find top N frequently occurring words in Dict
// input: Dictionary, array of WFreqs, size of array
// returns: #WFreqs in array, modified array
int findTopN(Dict d, WFreq *wfs, int n) {
    // TODO
    return 0;
}

// print a dictionary
void showDict(Dict d) {
    // TODO if you need to display Dict's for debugging
    recShow(d->tree, 0);
    return;
}

// referenced from https://www.geeksforgeeks.org/print-binary-tree-2-dimensions/
void recShow(Link node, int space) {
    // Base case
    if (node == NULL)
        return;

    // Increase distance between levels
    space += 10;

    // Process right child first
    recShow(node->right, space);

    // Print current node after space
    // count
    printf("\n");
    for (int i = 10; i < space; i++)
        printf(" ");
    printf("%s, %d, %d\n", node->data.word, node->height, height(node->left) - height(node->right));

    // Process left child
    recShow(node->left, space);
}

// Returns  the height of a subtree while assuming that the height field
// of the root node of the subtree is correct
int height(Link n) {
    if (n == NULL) {
        return -1;
    } else {
        return n->height;
    }
}

int max(int a, int b) {
    return a > b ? a : b;
}

// rotate left/right
// referenced from lecture slides
Link rotateLeft(Link n) {
    if (n == NULL || n->right == NULL) {
        return n;
    } else {
        Link x = n->right;
        Link temp = x->left;
        x->left = n;
        n->right = temp;

        // update heights
        n->height = 1 + max(height(n->left), height(n->right));
        x->height = 1 + max(height(x->left), height(x->right));

        return x;
    }
}

Link rotateRight(Link n) {
    if (n == NULL || n->left == NULL) {  //empty or has nothing to rotate to the right
        return n;
    } else {
        Link x = n->left;
        Link temp = x->right;
        x->right = n;
        n->left = temp;

        // update heights
        n->height = 1 + max(height(n->left), height(n->right));  // must update n first bc n is subtree of x now
        x->height = 1 + max(height(x->left), height(x->right));

        return x;
    }
}