#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "stack.h"

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

static int wordCount(const char *str) {
    int count = 0, inWord = 0;
    for (; *str; str++) {
        if (*str != ' ' && *str != '\t') {
            if (!inWord) { count++; inWord = 1; }
        } else {
            inWord = 0;
        }
    }
    return count;
}

static TStack *newStackNode(const char *name, const char *def,
                             const char *DoB, const char *DoD) {
    TStack *node = (TStack *)malloc(sizeof(TStack));
    if (!node) { perror("malloc"); exit(EXIT_FAILURE); }
    strncpy(node->name,       name, sizeof(node->name) - 1);
    strncpy(node->definition, def,  sizeof(node->definition) - 1);
    strncpy(node->DoB,        DoB,  sizeof(node->DoB) - 1);
    strncpy(node->DoD,        DoD,  sizeof(node->DoD) - 1);
    node->name[sizeof(node->name)-1]             = '\0';
    node->definition[sizeof(node->definition)-1] = '\0';
    node->DoB[sizeof(node->DoB)-1]               = '\0';
    node->DoD[sizeof(node->DoD)-1]               = '\0';
    node->next = NULL;
    return node;
}

TStack* pushStack(TStack *stk, const char *name, const char *def,
                  const char *DoB, const char *DoD) {
    TStack *node = newStackNode(name, def, DoB, DoD);
    node->next = stk;
    return node;
}

void printStack(TStack *stk) {
    if (!stk) {
        printf("  (empty)\n");
        return;
    }
    int i = 1;
    for (TStack *cur = stk; cur; cur = cur->next, i++)
        printf("  [%d] Name: %-30s  DoB: %-10s  DoD: %-10s\n"
               "       Def : %s\n",
               i, cur->name, cur->DoB, cur->DoD, cur->definition);
}

void freeStack(TStack *stk) {
    while (stk) { TStack *tmp = stk->next; free(stk); stk = tmp; }
}

void freeQueue(TQueue *q) {
    if (!q) return;
    TList *cur = q->front;
    while (cur) { TList *tmp = cur->next; free(cur); cur = tmp; }
    free(q);
}


TStack* toStack(TList *merged) {
    TStack *stk = NULL;
    int n = 0;
    TList *cur = merged;
    while (cur) { n++; cur = cur->next; }

    TList **arr = (TList **)malloc(n * sizeof(TList *));
    if (!arr) { perror("malloc"); exit(EXIT_FAILURE); }

    cur = merged;
    for (int i = 0; i < n; i++, cur = cur->next)
        arr[i] = cur;

    for (int i = n - 1; i >= 0; i--)
        stk = pushStack(stk, arr[i]->name, arr[i]->definition,
                        arr[i]->DoB, arr[i]->DoD);
    free(arr);
    return stk;
}

TStack* getInfoPersonality(TStack *stk, char *name) {
    if (!stk)
        return NULL;

    if (strcasestr_impl(stk->name, name) != NULL) {
        printf("  Found  : %s\n", stk->name);
        printf("  Def    : %s\n", stk->definition);
        printf("  DoB    : %s\n", stk->DoB);
        printf("  DoD    : %s\n", stk->DoD);
        return pushStack(NULL, stk->name, stk->definition,
                         stk->DoB, stk->DoD);
    }

    return getInfoPersonality(stk->next, name);
}

static TStack* sortedInsert(TStack *sorted, TStack *node) {
    node->next = NULL;

    if (!sorted || strcasecmp(node->name, sorted->name) <= 0) {
        node->next = sorted;
        return node;
    }

    TStack *cur = sorted;
    while (cur->next && strcasecmp(node->name, cur->next->name) > 0)
        cur = cur->next;

    node->next = cur->next;
    cur->next  = node;
    return sorted;
}

TStack* sortNameStack(TStack *s) {
    TStack *sorted = NULL;
    while (s) {
        TStack *next = s->next;
        sorted = sortedInsert(sorted, s);
        s = next;
    }
    return sorted;
}

TStack* deleteName(TStack *stk, char *name) {
    if (!stk)
        return NULL;

    if (strcasestr_impl(stk->name, name) != NULL) {
        TStack *rest = stk->next;
        free(stk);
        return rest;
    }

    stk->next = deleteName(stk->next, name);
    return stk;
}

TStack* updateStack(TStack *stk, char *name, char *def,
                    char *DoB, char *DoD) {
    if (!stk)
        return NULL;

    if (strcasestr_impl(stk->name, name) != NULL) {
        if (def[0] != '\0')
            strncpy(stk->definition, def, sizeof(stk->definition) - 1);
        if (DoB[0] != '\0')
            strncpy(stk->DoB, DoB, sizeof(stk->DoB) - 1);
        if (DoD[0] != '\0')
            strncpy(stk->DoD, DoD, sizeof(stk->DoD) - 1);
        return stk;
    }

    stk->next = updateStack(stk->next, name, def, DoB, DoD);
    return stk;
}

TQueue* stackToQueue(TStack *stk) {
    TQueue *q = (TQueue *)malloc(sizeof(TQueue));
    if (!q) { perror("malloc"); exit(EXIT_FAILURE); }
    q->front = q->rear = NULL;

    TStack *sorted = sortNameStack(stk);

    for (TStack *cur = sorted; cur; cur = cur->next) {
        TList *node = (TList *)malloc(sizeof(TList));
        if (!node) { perror("malloc"); exit(EXIT_FAILURE); }
        strncpy(node->name,       cur->name,       sizeof(node->name) - 1);
        strncpy(node->definition, cur->definition, sizeof(node->definition) - 1);
        strncpy(node->DoB,        cur->DoB,        sizeof(node->DoB) - 1);
        strncpy(node->DoD,        cur->DoD,        sizeof(node->DoD) - 1);
        node->next = NULL;

        if (!q->rear)
            q->front = q->rear = node;
        else {
            q->rear->next = node;
            q->rear       = node;
        }
    }
    return q;
}

TList* stackToList(TStack *stk) {
    TList *head = NULL, *tail = NULL;

    TStack *sorted = sortNameStack(stk);

    for (TStack *cur = sorted; cur; cur = cur->next) {
        TList *node = (TList *)malloc(sizeof(TList));
        if (!node) { perror("malloc"); exit(EXIT_FAILURE); }
        strncpy(node->name,       cur->name,       sizeof(node->name) - 1);
        strncpy(node->definition, cur->definition, sizeof(node->definition) - 1);
        strncpy(node->DoB,        cur->DoB,        sizeof(node->DoB) - 1);
        strncpy(node->DoD,        cur->DoD,        sizeof(node->DoD) - 1);
        node->next = NULL;
        node->prev = tail;

        if (!tail)
            head = node;
        else
            tail->next = node;

        tail = node;
    }
    return head;
}

TStack* addNameStack(TStack *stk, char *name, char *definition,
                     char *DoB, char *DoD) {
    TStack *node = newStackNode(name, definition, DoB, DoD);
    return sortedInsert(stk, node);
}

static TStack* sortedInsertByDefLen(TStack *sorted, TStack *node) {
    node->next = NULL;
    int nw = wordCount(node->definition);

    if (!sorted || nw <= wordCount(sorted->definition)) {
        node->next = sorted;
        return node;
    }

    TStack *cur = sorted;
    while (cur->next && nw > wordCount(cur->next->definition))
        cur = cur->next;

    node->next = cur->next;
    cur->next  = node;
    return sorted;
}

TStack* definitionStack(TStack *stk) {
    TStack *sorted = NULL;
    while (stk) {
        TStack *next = stk->next;
        sorted = sortedInsertByDefLen(sorted, stk);
        stk = next;
    }
    return sorted;
}

void pronunciationStack(TStack *stk, TStack **shortStk, TStack **longStk) {
    *shortStk = NULL;
    *longStk  = NULL;

    while (stk) {
        TStack *next = stk->next;
        if (wordCount(stk->definition) <= 5) {
            stk->next = *shortStk;
            *shortStk = stk;
        } else {
            stk->next = *longStk;
            *longStk  = stk;
        }
        stk = next;
    }
}

char* getSmallest(TStack *stk) {
    if (!stk)
        return NULL;

    char *smallest = stk->definition;
    int   minWords = wordCount(stk->definition);

    for (TStack *cur = stk->next; cur; cur = cur->next) {
        int w = wordCount(cur->definition);
        if (w < minWords) {
            minWords = w;
            smallest = cur->definition;
        }
    }
    return smallest;
}

void continuousSearch(TStack *stk) {
    int found = 0;
    for (TStack *a = stk; a; a = a->next) {
        for (TStack *b = a->next; b; b = b->next) {
            if (strcmp(a->DoB, b->DoD) <= 0 &&
                strcmp(b->DoB, a->DoD) <= 0) {
                printf("  Overlap: [%s  %s-%s]  <->  [%s  %s-%s]\n",
                       a->name, a->DoB, a->DoD,
                       b->name, b->DoB, b->DoD);
                found = 1;
            }
        }
    }
    if (!found)
        printf("  No overlapping date ranges found.\n");
}

bool isPersonalityKilled(char *word) {
    return strcasestr_impl(word, "killed") != NULL;
}

static TStack* insertAtBottom(TStack *stk, TStack *node) {
    if (!stk) {
        node->next = NULL;
        return node;
    }
    stk->next = insertAtBottom(stk->next, node);
    return stk;
}

TStack* recRevStack(TStack *stk) {
    if (!stk || !stk->next)
        return stk;

    TStack *rest = recRevStack(stk->next);
    stk->next = NULL;
    return insertAtBottom(rest, stk);
}
