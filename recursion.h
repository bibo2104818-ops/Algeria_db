#ifndef RECURSION_H
#define RECURSION_H

#include <stdio.h>
#include <stdbool.h>

int   countOccurence(FILE *f, char *name);
FILE* removeOccurence(FILE *f, char *word);
FILE* replaceOccurence(FILE *f, char *name, char *DoB, char *DoD);
void  namePermutation(char *name);
void  subseqName(char *word);
void  longestSubyear(char *date1, char *date2);
int   distinctSubseqWord(char *event);
bool  isPalindromeWord(char *event);

#endif