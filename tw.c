// COMP2521 20T2 Assignment 1
// tw.c ... compute top N most frequent words in file F
// Usage: ./tw [Nwords] File

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Dict.h"
#include "WFreq.h"
#include "stemmer.h"

#define STOPWORDS "stopwords"
#define MAXLINE 1000
#define MAXWORD 100

#define STARTING "*** START OF"
#define ENDING "*** END OF"

#define isWordChar(c) (isalnum(c) || (c) == '\'' || (c) == '-')

int numOfNodes(Dict d);
void mergefreq(WFreq arr[], int l, int m, int r);
void mergeSortfreq(WFreq arr[], int l, int r);
void mergeword(WFreq arr[], int l, int m, int r);
void mergeSortword(WFreq arr[], int l, int r);

int main(int argc, char *argv[]) {
    char *fileName;  // name of file containing book text
    int nWords;      // number of top frequency words to show

    char line[MAXLINE];  // current input line
    char word[MAXWORD];  // current word

    // process command-line args
    switch (argc) {
        case 2:
            nWords = 10;
            fileName = argv[1];
            break;
        case 3:
            nWords = atoi(argv[1]);
            if (nWords < 10) nWords = 10;
            fileName = argv[2];
            break;
        default:
            fprintf(stderr, "Usage: %s [Nwords] File\n", argv[0]);
            exit(EXIT_FAILURE);
    }

    // build stopword dictionary
    FILE *fp;
    fp = fopen(STOPWORDS, "r");
    // error
    if (fp == NULL) {
        fprintf(stderr, "Can't open stopwords\n");
        return EXIT_FAILURE;
    }
    Dict stopwords = newDict();  // dictionary of stopwords
    while (fgets(word, MAXWORD, fp) != NULL) {
        char *string = malloc(sizeof(char) * (strlen(word) + 1));
        strncpy(string, word, strlen(word) - 1);
        DictInsert(stopwords, string);
    }

    // scan File, up to start of text
    // reads file until "*** START OF" is encountered
    FILE *txt;
    txt = fopen(fileName, "r");
    // error
    if (txt == NULL) {
        fprintf(stderr, "Can't open %s\n", fileName);
        return EXIT_FAILURE;
    }
    bool seeStart = false;
    while (fgets(line, MAXLINE, txt) != NULL) {
        if (strncmp(line, STARTING, strlen(STARTING)) == 0) {
            seeStart = true;
            break;
        }
    }
    if (!seeStart) {
        fprintf(stderr, "Not a Project Gutenberg book\n");
        return EXIT_FAILURE;
    }
    Dict wordDict = newDict();  // Book dictionary
    // scan File reading words and accumualting counts
    while (fgets(line, MAXLINE, txt) != NULL) {
        if (strncmp(line, ENDING, strlen(ENDING)) == 0) {  // break if reaches ENDING
            break;
        }
        char *linestring = malloc(sizeof(char) * (strlen(line) + 1));
        strcpy(linestring, line);
        for (int i = 0; i < strlen(linestring); i++) {
            linestring[i] = tolower(linestring[i]);
        }
        for (int i = 0; i < strlen(linestring); i++) {
            if (!isWordChar(linestring[i])) {
                linestring[i] = ' ';
            }
        }
        char *token = strtok(linestring, " ");
        while (token != NULL) {
            if (strlen(token) > 1) {                       // is a valid word
                if (DictFind(stopwords, token) == NULL) {  // doenst exist in stopwords dict
                    char *addWord = malloc(sizeof(char) * (strlen(token) + 1));
                    strcpy(addWord, token);
                    int k = stem(addWord, 0, strlen(addWord) - 1);
                    addWord[k + 1] = '\0';
                    WFreq *exist = DictFind(wordDict, addWord);
                    if (exist == NULL) {
                        DictInsert(wordDict, addWord);
                    } else {
                        assert(strcmp(exist->word, addWord) == 0);
                        // increment WFreq freq
                        exist->freq++;
                        free(addWord);
                    }
                }
            }
            token = strtok(NULL, " ");
        }
        free(linestring);
    }

    // compute and display the top N words
    int size = numOfNodes(wordDict);
    WFreq results[size];  // array that contains all node WFreq in wordDict
    findTopN(wordDict, results, size);

    // bubbleSort(results, size);

    // }

    // merge sort
    mergeSortword(results, 0, size - 1);
    mergeSortfreq(results, 0, size - 1);
    for (int i = 0; i < nWords; i++) {
        printf("%7d %s\n", results[i].freq, results[i].word);
    }
    return EXIT_SUCCESS;
}


// merge sort referenced from https://www.geeksforgeeks.org/merge-sort/

void mergefreq(WFreq arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    /* create temp arrays */
    WFreq L[n1], R[n2];

    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    /* Merge the temp arrays back into arr[l..r]*/
    i = 0;  // Initial index of first subarray
    j = 0;  // Initial index of second subarray
    k = l;  // Initial index of merged subarray
    while (i < n1 && j < n2) {
        if (L[i].freq > R[j].freq) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    /* Copy the remaining elements of L[], if there 
       are any */
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    /* Copy the remaining elements of R[], if there 
       are any */
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

/* l is for left index and r is right index of the 
   sub-array of arr to be sorted */
void mergeSortfreq(WFreq arr[], int l, int r) {
    if (l < r) {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l + (r - l) / 2;

        // Sort first and second halves
        mergeSortfreq(arr, l, m);
        mergeSortfreq(arr, m + 1, r);

        mergefreq(arr, l, m, r);
    }
}

void mergeword(WFreq arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    /* create temp arrays */
    WFreq L[n1], R[n2];

    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    /* Merge the temp arrays back into arr[l..r]*/
    i = 0;  // Initial index of first subarray
    j = 0;  // Initial index of second subarray
    k = l;  // Initial index of merged subarray
    while (i < n1 && j < n2) {
        int cmp = strcmp(L[i].word, R[j].word);
        if (cmp > 0) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    /* Copy the remaining elements of L[], if there 
       are any */
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    /* Copy the remaining elements of R[], if there 
       are any */
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

/* l is for left index and r is right index of the 
   sub-array of arr to be sorted */
void mergeSortword(WFreq arr[], int l, int r) {
    if (l < r) {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l + (r - l) / 2;

        // Sort first and second halves
        mergeSortword(arr, l, m);
        mergeSortword(arr, m + 1, r);

        mergeword(arr, l, m, r);
    }
}