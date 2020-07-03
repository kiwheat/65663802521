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

int main(int argc, char *argv[]) {
    // TODO ... add any variables you need

    FILE *in;  // currently open file

    Dict wfreqs;     // dictionary of words from book
    WFreq *results;  // array of top N (word,freq) pairs
                     // (dynamically allocated)

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
    int a = 0;
    // scan File reading words and accumualting counts
    while (fgets(line, MAXLINE, txt) != NULL && a < 35) {
        if (strncmp(line, ENDING, strlen(ENDING)) == 0) {  // break if reaches ENDING
            break;
        }
        char *linestring = malloc(sizeof(char) * (strlen(line) + 1));
        strcpy(linestring, line);
        for (int i = 0; i < strlen(linestring); i++) {
            linestring[i] = tolower(linestring[i]);
        }
        a++;
        for (int i = 0; i < strlen(linestring); i++) {
            if (!isWordChar(linestring[i])) {
                linestring[i] = ' ';
            }
        }
        char *token = strtok(linestring, " ");
        while (token != NULL) {
            if (strlen(token) > 1) {                       // is a valid word
                if (DictFind(stopwords, token) == NULL) {  // doenst exist in dict
                    printf("%s\n", token);
                }
            }

            token = strtok(NULL, " ");
        }
    }

    // TODO

    // compute and display the top N words

    // TODO

    // done
    return EXIT_SUCCESS;
}
