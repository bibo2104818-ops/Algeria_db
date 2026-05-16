#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "recursion.h"

static char *strcasestr_impl(const char *haystack, const char *needle) {
    if (!*needle)
        return (char *)haystack;

    for (; *haystack; haystack++) {
        if (tolower((unsigned char)*haystack) == tolower((unsigned char)*needle)) {
            const char *h = haystack, *n = needle;
            while (*h && *n &&
                   tolower((unsigned char)*h) == tolower((unsigned char)*n)) {
                h++; n++;
            }
            if (!*n)
                return (char *)haystack;
        }
    }
    return NULL;
}

int countOccurence(FILE *f, char *name) {
    char tmp[100];

    if (f == NULL)
        return 0;

    if (fgets(tmp, sizeof(tmp), f) == NULL)
        return 0;

    tmp[strcspn(tmp, "\n")] = '\0';

    if (strcasestr_impl(tmp, name) != NULL)
        return 1 + countOccurence(f, name);

    return countOccurence(f, name);
}

static void removeRec(FILE *in, FILE *out, char *word) {
    char tmp[100];

    if (fgets(tmp, sizeof(tmp), in) == NULL)
        return;

    if (strcasestr_impl(tmp, word) == NULL)
        fputs(tmp, out);

    removeRec(in, out, word);
}

FILE* removeOccurence(FILE *f, char *word) {
    FILE *out = tmpfile();
    if (!out)
        return NULL;

    removeRec(f, out, word);
    rewind(out);
    return out;
}

static void replaceRec(FILE *in, FILE *out, char *name, char *DoB, char *DoD) {
    char tmp[100];

    if (fgets(tmp, sizeof(tmp), in) == NULL)
        return;

    tmp[strcspn(tmp, "\n")] = '\0';

    if (strcasestr_impl(tmp, name) != NULL)
        fprintf(out, "%s=%s=%s\n", name, DoB, DoD);
    else
        fprintf(out, "%s\n", tmp);

    replaceRec(in, out, name, DoB, DoD);
}

FILE* replaceOccurence(FILE *f, char *name, char *DoB, char *DoD) {
    FILE *out = tmpfile();
    if (!out)
        return NULL;

    replaceRec(f, out, name, DoB, DoD);
    rewind(out);
    return out;
}

static void swap(char *a, char *b) {
    char t = *a;
    *a = *b;
    *b = t;
}

static void permute(char *str, int l, int r) {
    if (l == r) {
        printf("%s\n", str);
        return;
    }

    for (int i = l; i <= r; i++) {
        swap(&str[l], &str[i]);
        permute(str, l + 1, r);
        swap(&str[l], &str[i]);
    }
}

void namePermutation(char *name) {
    permute(name, 0, strlen(name) - 1);
}

static void subseq(char *str, char *out, int i, int j) {
    if (str[i] == '\0') {
        if (j > 0) {
            out[j] = '\0';
            printf("%s\n", out);
        }
        return;
    }
    out[j] = str[i];
    subseq(str, out, i + 1, j + 1);
    subseq(str, out, i + 1, j);
}

void subseqName(char *word) {
    char out[100];
    memset(out, 0, sizeof(out));
    subseq(word, out, 0, 0);
}

static int lcsLength(char *a, char *b) {
    if (*a == '\0' || *b == '\0')
        return 0;
    if (*a == *b)
        return 1 + lcsLength(a + 1, b + 1);
    int x = lcsLength(a + 1, b);
    int y = lcsLength(a, b + 1);
    return x > y ? x : y;
}

static void lcsPrint(char *a, char *b) {
    if (*a == '\0' || *b == '\0')
        return;
    if (*a == *b) {
        printf("%c", *a);
        lcsPrint(a + 1, b + 1);
    } else {
        if (lcsLength(a + 1, b) >= lcsLength(a, b + 1))
            lcsPrint(a + 1, b);
        else
            lcsPrint(a, b + 1);
    }
}

void longestSubyear(char *date1, char *date2) {
    lcsPrint(date1, date2);
}

int distinctSubseqWord(char *event) {
    if (*event == '\0')
        return 1;

    return 2 * distinctSubseqWord(event + 1);
}

static bool isPal(char *str, int l, int r) {
    if (l >= r)
        return true;
    if (str[l] != str[r])
        return false;
    return isPal(str, l + 1, r - 1);
}

bool isPalindromeWord(char *event) {
    return isPal(event, 0, strlen(event) - 1);
}
