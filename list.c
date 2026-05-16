#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "list.h"



TList* createNode(char *name, char *definition, char *DoB, char *DoD) {
    TList *newNode = (TList*)malloc(sizeof(TList));
    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        return NULL;   
    }
    strcpy(newNode->name, name);
    strcpy(newNode->definition, definition);
    strcpy(newNode->DoB, DoB);
    strcpy(newNode->DoD, DoD);
    newNode->next = NULL;
    newNode->prev = NULL;
    return newNode;
}

// BUG 1 FIXED: newNode was never linked to the list
TList* addEndLLL(TList *head, char *name, char *definition, char *DoB, char *DoD) {
    TList *newNode = createNode(name, definition, DoB, DoD);
    if (newNode == NULL) return head;

    if (head == NULL) return newNode;  // empty list

    TList *temp = head;
    while (temp->next != NULL)
        temp = temp->next;

    temp->next = newNode;   // actually link the new node
    newNode->prev = temp;
    return head;
}

// BUG 2 FIXED: p->next accessed after p became NULL (segfault)
void displayLLL(TList *head) {
    TList *p = head;
    while (p != NULL) {
        printf("Name: %s\n", p->name);
        printf("Definition: %s\n", p->definition);
        printf("Date of Birth: %s\n", p->DoB);
        printf("Date of Death: %s\n", p->DoD);
        printf("-------------------------\n");
        p = p->next;   // removed the extra p = p->next after the loop
    }
}

void freeList(TList *head) {
    TList *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

TList* getPersonality(FILE *f) {
    char *name, *definition;
    char line[600];
    TList *head = NULL;
    while (fgets(line, sizeof(line), f) != NULL) {
        if (strchr(line, '=') != NULL) {
            name = strtok(line, "=");
            definition = strtok(NULL, "\n");
            if (name && definition)
                head = addEndLLL(head, name, definition, "", "");
        }
    }
    return head;
}

TList* getDatePersonality(FILE *f) {
    char line[140];
    char name[100], DoB[20], DoD[20];
    TList *head = NULL;
    while (fgets(line, sizeof(line), f) != NULL) {
        if (strchr(line, '=') != NULL && strchr(line, '{') != NULL) {
            sscanf(line, "%99[^=]=%*[^{}]{%19[^-]-%19[^}]}", name, DoB, DoD);
            head = addEndLLL(head, name, "", DoB, DoD);
        }
    }
    return head;
}

void getInfoByDates(TList *s, TList *DoB) {
    char date[20];
    printf("Enter the date of birth:");
    scanf("%19s", date);
    TList *d = DoB;
    while (d != NULL) {
        if (strcmp(d->DoB, date) == 0) {
            TList *p = s;
            while (p != NULL) {
                if (strcmp(p->name, d->name) == 0) {
                    printf("Name: %s\n", p->name);
                    printf("Definition: %s\n", p->definition);
                    printf("Date of Birth: %s\n", p->DoB);
                    printf("Date of Death: %s\n", p->DoD);
                    return;
                }
                p = p->next;
            }
        }
        d = d->next;
    }
    printf("No personality found with this date of birth.\n");
}

void getInfoByDates2(TList *s, TList *DoD) {
    char date[20];
    printf("Enter date of death: ");
    scanf("%19s", date);
    TList *d = DoD;
    while (d != NULL) {
        if (strcmp(d->DoD, date) == 0) {
            TList *p = s;
            while (p != NULL) {
                if (strcmp(p->name, d->name) == 0) {
                    printf("Name: %s\n", p->name);
                    printf("Definition: %s\n", p->definition);
                    printf("Date of Birth: %s\n", d->DoB);
                    printf("Date of Death: %s\n", d->DoD);
                    return;
                }
                p = p->next;
            }
        }
        d = d->next;
    }
    printf("No personality found with this date of death.\n");
}

TList* sortWord(TList *syn) {
    TList *currentNode, *nextNode;
    char tempName[100], tempDefinition[500], tempDoB[20], tempDoD[20];
    if (syn == NULL) return NULL;
    for (currentNode = syn; currentNode->next != NULL; currentNode = currentNode->next) {
        for (nextNode = currentNode->next; nextNode != NULL; nextNode = nextNode->next) {
            if (strcmp(currentNode->name, nextNode->name) > 0) {
                strcpy(tempName, currentNode->name);
                strcpy(tempDefinition, currentNode->definition);
                strcpy(tempDoB, currentNode->DoB);
                strcpy(tempDoD, currentNode->DoD);
                strcpy(currentNode->name, nextNode->name);
                strcpy(currentNode->definition, nextNode->definition);
                strcpy(currentNode->DoB, nextNode->DoB);
                strcpy(currentNode->DoD, nextNode->DoD);
                strcpy(nextNode->name, tempName);
                strcpy(nextNode->definition, tempDefinition);
                strcpy(nextNode->DoB, tempDoB);
                strcpy(nextNode->DoD, tempDoD);
            }
        }
    }
    return syn;
}

TList* sortWord2(TList *syn) {
    TList *currentNode, *nextNode;
    char tempName[100], tempDefinition[500], tempDoB[20], tempDoD[20];
    for (currentNode = syn; currentNode->next != NULL; currentNode = currentNode->next) {
        for (nextNode = currentNode->next; nextNode != NULL; nextNode = nextNode->next) {
            if (strlen(currentNode->name) > strlen(nextNode->name)) {
                strcpy(tempName, currentNode->name);
                strcpy(currentNode->name, nextNode->name);
                strcpy(nextNode->name, tempName);
                strcpy(tempDefinition, currentNode->definition);
                strcpy(currentNode->definition, nextNode->definition);
                strcpy(nextNode->definition, tempDefinition);
                strcpy(tempDoB, currentNode->DoB);
                strcpy(currentNode->DoB, nextNode->DoB);
                strcpy(nextNode->DoB, tempDoB);
                strcpy(tempDoD, currentNode->DoD);
                strcpy(currentNode->DoD, nextNode->DoD);
                strcpy(nextNode->DoD, tempDoD);
            }
        }
    }
    return syn;
}

int calculateAge(TList *p) {
    int birth = atoi(p->DoB);
    int death = atoi(p->DoD);
    return death - birth;
}

TList* sortPersonality(TList *syn) {
    TList *currentNode, *nextNode;
    char tempName[100], tempDefinition[500], tempDoB[20], tempDoD[20];
    for (currentNode = syn; currentNode->next != NULL; currentNode = currentNode->next) {
        for (nextNode = currentNode->next; nextNode != NULL; nextNode = nextNode->next) {
            if (calculateAge(currentNode) > calculateAge(nextNode)) {
                strcpy(tempName, currentNode->name);
                strcpy(currentNode->name, nextNode->name);
                strcpy(nextNode->name, tempName);
                strcpy(tempDefinition, currentNode->definition);
                strcpy(currentNode->definition, nextNode->definition);
                strcpy(nextNode->definition, tempDefinition);
                strcpy(tempDoB, currentNode->DoB);
                strcpy(currentNode->DoB, nextNode->DoB);
                strcpy(nextNode->DoB, tempDoB);
                strcpy(tempDoD, currentNode->DoD);
                strcpy(currentNode->DoD, nextNode->DoD);
                strcpy(nextNode->DoD, tempDoD);
            }
        }
    }
    return syn;  
}

TList* deleteFromList(TList *head, char *name) {
    TList *temp = head, *prevNode = NULL;
    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0) {
            if (prevNode == NULL)
                head = temp->next;
            else
                prevNode->next = temp->next;
            free(temp);
            return head;
        }
        prevNode = temp;
        temp = temp->next;
    }
    return head;
}

TList* deletepersonality(FILE *f, TList *s, TList *a, char *name) {
    s = deleteFromList(s, name);
    a = deleteFromList(a, name);
    printf("Personality deleted if it existed.\n");
    return s;
}

TList* updatePersonality(FILE *f, TList *s, TList *a, char *name, char *definition, char *DoB, char *DoD) {
    TList *temp = s;
    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0) {
            strcpy(temp->definition, definition);
            break;
        }
        temp = temp->next;
    }
    temp = a;
    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0) {
            strcpy(temp->DoB, DoB);
            strcpy(temp->DoD, DoD);
            break;
        }
        temp = temp->next;
    }
    printf("Personality updated.\n");
    return s;
}

TList* similarPersonality(TList *s, char *word) {
    TList *temp = s, *target = NULL, *result = NULL;
    while (temp != NULL) {
        if (strcmp(temp->name, word) == 0) { target = temp; break; }
        temp = temp->next;
    }
    if (target == NULL) { printf("Personality not found!\n"); return NULL; }
    temp = s;
    while (temp != NULL) {
        if (strcmp(temp->name, word) != 0) {
            if (strcmp(temp->DoB, target->DoB) == 0 || strcmp(temp->DoD, target->DoD) == 0)
                result = addEndLLL(result, temp->name, temp->definition, temp->DoB, temp->DoD);
        }
        temp = temp->next;
    }
    return result;
}

TList* countPersonality(TList *s, char *date) {
    TList *temp = s, *result = NULL;
    while (temp != NULL) {
        if (strcmp(temp->DoB, date) == 0 || strcmp(temp->DoD, date) == 0)
            result = addEndLLL(result, temp->name, temp->definition, temp->DoB, temp->DoD);
        temp = temp->next;
    }
    return result;
}

TQueue* createQueue() {
    TQueue *q = malloc(sizeof(TQueue));
    q->front = NULL;
    q->rear = NULL;
    return q;
}

void enqueue(TQueue *q, char *name, char *definition, char *DoB, char *DoD) {
    TList *newNode = createNode(name, definition, DoB, DoD);
    if (q->rear == NULL) {
        q->front = newNode;
        q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
}

TQueue* toQueue(TList *merged) {
    TQueue *q = createQueue();
    TList *temp = merged;
    while (temp != NULL) {
        enqueue(q, temp->name, temp->definition, temp->DoB, temp->DoD);
        temp = temp->next;
    }
    return q;
}
TList* addPersonality(TList *list, TList *anchor, char *name, char *dob, char *dod) {
    TList *new_node = (TList*)malloc(sizeof(TList));
    if (new_node == NULL) return list; // Out-of-memory safety guard

    if (name != NULL) {
        strncpy(new_node->name, name, sizeof(new_node->name) - 1);
        new_node->name[sizeof(new_node->name) - 1] = '\0'; // Ensure absolute null-termination
    }

    new_node->next = NULL;

    if (list == NULL) {
        return new_node;
    }

    new_node->next = list;
    return new_node;
}

TList* addEvents(TList *b, char *nameEvent, char *date) {
    b = addEndLLL(b, nameEvent, "", date, "");
    printf("Event added.\n");
    return b;
}

TList* mergeNodes(TList *s, TList *a) {
    TList *mergedHead = NULL, *last = NULL, *tempS = s;
    while (tempS != NULL) {
        TList *tempA = a;
        char *birth = "", *death = "";
        while (tempA != NULL) {
            if (strcmp(tempS->name, tempA->name) == 0) {
                birth = tempA->DoB;
                death = tempA->DoD;
                break;
            }
            tempA = tempA->next;
        }
        TList *newNode = createNode(tempS->name, tempS->definition, birth, death);
        if (mergedHead == NULL) {
            mergedHead = newNode;
        } else {
            last->next = newNode;
            newNode->prev = last;
        }
        last = newNode;
        tempS = tempS->next;
    }
    return mergedHead;
}

TList* merge2Nodes(TList *s, TList *a) {
    TList *head = mergeNodes(s, a);
    if (head == NULL) return NULL;
    TList *temp = head;
    while (temp->next != NULL) temp = temp->next;
    temp->next = head;
    return head;
}

int isPalindrome(char *str) {
    int l = 0, h = strlen(str) - 1;
    while (h > l) {
        if (tolower(str[l++]) != tolower(str[h--])) return 0;
    }
    return 1;
}

TList* palindromeName(TList *s) {
    TList *res = NULL, *curr = s;
    while (curr != NULL) {
        if (isPalindrome(curr->name))
            res = insertSorted(res, curr->name, curr->definition, curr->DoB, curr->DoD);
        curr = curr->next;
    }
    return res;
}

int countWords(char *str) {
    int count = 1;
    for (int i = 0; str[i]; i++) if (str[i] == ' ') count++;
    return count;
}

TQueue* sName(TList *s) {
    TList *curr, *index;
    char tempName[100], tempDef[500], tempDoB[20], tempDoD[20];
    for (curr = s; curr != NULL; curr = curr->next) {
        for (index = curr->next; index != NULL; index = index->next) {
            if (countWords(curr->name) > countWords(index->name)) {
                strcpy(tempName, curr->name); strcpy(curr->name, index->name); strcpy(index->name, tempName);
                strcpy(tempDef, curr->definition); strcpy(curr->definition, index->definition); strcpy(index->definition, tempDef);
                strcpy(tempDoB, curr->DoB); strcpy(curr->DoB, index->DoB); strcpy(index->DoB, tempDoB);
                strcpy(tempDoD, curr->DoD); strcpy(curr->DoD, index->DoD); strcpy(index->DoD, tempDoD);
            }
        }
    }
    return toQueue(s);
}

TQueue* ageP(TList *a) {
    if (a == NULL) return NULL;
    TList *tempList = NULL, *curr = a;
    while (curr != NULL) {
        tempList = addEndLLL(tempList, curr->name, curr->definition, curr->DoB, curr->DoD);
        curr = curr->next;
    }
    TList *i, *j;
    char tempName[100], tempDef[500], tempB[20], tempD[20];
    for (i = tempList; i != NULL && i->next != NULL; i = i->next) {
        for (j = i->next; j != NULL; j = j->next) {
            if (calculateAge(i) > calculateAge(j)) {
                strcpy(tempName, i->name); strcpy(tempDef, i->definition);
                strcpy(tempB, i->DoB); strcpy(tempD, i->DoD);
                strcpy(i->name, j->name); strcpy(i->definition, j->definition);
                strcpy(i->DoB, j->DoB); strcpy(i->DoD, j->DoD);
                strcpy(j->name, tempName); strcpy(j->definition, tempDef);
                strcpy(j->DoB, tempB); strcpy(j->DoD, tempD);
            }
        }
    }
    return toQueue(tempList);
}

// insertSorted — inserts a node in alphabetical order (used by palindromeName)
TList* insertSorted(TList *head, char *name, char *definition, char *DoB, char *DoD) {
    TList *newNode = createNode(name, definition, DoB, DoD);
    if (!head || strcmp(name, head->name) <= 0) {
        newNode->next = head;
        return newNode;
    }
    TList *cur = head;
    while (cur->next && strcmp(name, cur->next->name) > 0)
        cur = cur->next;
    newNode->next = cur->next;
    cur->next = newNode;
    return head;
}

// displayList — alias kept consistent with list.h declaration
void displayList(TList *head) {
    TList *p = head;
    while (p != NULL) {
        printf("Name: %s\n", p->name);
        printf("Definition: %s\n", p->definition);
        printf("Date of Birth: %s\n", p->DoB);
        printf("Date of Death: %s\n", p->DoD);
        printf("-------------------------\n");
        p = p->next;
    }
}

void displayQueue(TQueue *q) {
    if (!q || !q->front) { printf("  (empty)\n"); return; }
    TList *cur = q->front;
    int i = 1;
    while (cur) {
        printf("  [%d] %s | DoB: %s | DoD: %s\n", i++, cur->name, cur->DoB, cur->DoD);
        cur = cur->next;
    }
}
