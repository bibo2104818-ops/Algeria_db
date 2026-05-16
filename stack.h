#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdbool.h>
#include "types.h"

TStack* pushStack(TStack *stk, const char *name, const char *def, const char *DoB, const char *DoD);
void    printStack(TStack *stk);
void    freeStack(TStack *stk);

TStack* toStack(TList *merged);
TStack* getInfoPersonality(TStack *stk, char *name);
TStack* sortNameStack(TStack *s);
TStack* deleteName(TStack *stk, char *name);
TStack* updateStack(TStack *stk, char *name, char *def, char *DoB, char *DoD);
TQueue* stackToQueue(TStack *stk);
TList*  stackToList(TStack *stk);
TStack* addNameStack(TStack *stk, char *name, char *definition, char *DoB, char *DoD);
TStack* definitionStack(TStack *stk);
void    pronunciationStack(TStack *stk, TStack **shortStk, TStack **longStk);
char*   getSmallest(TStack *stk);
void    continuousSearch(TStack *stk);
bool    isPersonalityKilled(char *word);
TStack* recRevStack(TStack *stk);

#endif