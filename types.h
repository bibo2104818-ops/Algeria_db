#ifndef TYPES_H
#define TYPES_H

// ── Linked List Node ────────────────────────────────────────────────────────
typedef struct TList {
    char name[100];
    char definition[500];
    char DoB[20];
    char DoD[20];
    struct TList *next;
    struct TList *prev;
} TList;

// ── Queue (of TList nodes) ──────────────────────────────────────────────────
typedef struct {
    TList *front;
    TList *rear;
} TQueue;

// ── Stack Node ──────────────────────────────────────────────────────────────
typedef struct TStack {
    char name[100];
    char definition[500];
    char DoB[20];
    char DoD[20];
    struct TStack *next;
} TStack;

// ── BST Node ─────────────────────────────────────────────────────────────────
typedef struct TTree {
    char name[100];
    char DoB[20];
    char DoD[20];
    struct TTree *left;
    struct TTree *right;
} TTree;

#endif