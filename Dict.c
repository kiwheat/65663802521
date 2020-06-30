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
} DictNode;

struct _DictRep {
    Link tree;
};
//
// define functions
Link Insert(Link n, Link newNode);
void recShow(Link node, int space);
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

    d->tree = Insert(d->tree, newNode);
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

    return n;
}

// find Word in Dictionary
// return pointer to (word,freq) pair, if found
// otherwise return NULL
WFreq *DictFind(Dict d, char *w) {
    // TODO
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
    printf("%s\n", node->data.word);

    // Process left child
    recShow(node->left, space);
}