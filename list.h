#ifndef LIST_H
#define LIST_H
#include <stdio.h>
#include "types.h"

TList* createNode(char *name, char *definition, char *DoB, char *DoD);
TList* addEndLLL(TList *head, char *name, char *definition, char *DoB, char *DoD);
void   displayList(TList *head);
void   freeList(TList *head);
TList* getPersonality(FILE *f);
TList* getDatePersonality(FILE *f);
void   getInfoByDates(TList *s, TList *DoB);
void   getInfoByDates2(TList *s, TList *DoD);
TList* sortWord(TList *syn);
TList* sortWord2(TList *syn);
TList* sortPersonality(TList *syn);
int    calculateAge(TList *p);
TList* deletePersonality(FILE *f, TList *s, TList *a, char *name);
TList* updatePersonality(FILE *f, TList *s, TList *a, char *name, char *definition, char *DoB, char *DoD);
TList* similarPersonality(TList *s, char *word);
TList* countPersonality(TList *s, char *date);
TList* palindromeName(TList *s);
TList* insertSorted(TList *head, char *name, char *definition, char *DoB, char *DoD);
TList* mergeNodes(TList *s, TList *a);
TList* merge2Nodes(TList *s, TList *a);
TList* addPersonality(TList *s, TList *a, char *name, char *DoB, char *DoD);
TList* addEvents(TList *b, char *nameEvent, char *date);
TQueue* createQueue();
void    enqueue(TQueue *q, char *name, char *definition, char *DoB, char *DoD);
void    displayQueue(TQueue *q);
void    freeQueue(TQueue *q);
TQueue* sName(TList *s);
TQueue* ageP(TList *a);
TQueue* toQueue(TList *merged);

#endif
