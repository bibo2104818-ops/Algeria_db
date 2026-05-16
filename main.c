#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "list.h"
#include "stack.h"
#include "recursion.h"
#include "binary_tree.h"

// ─── Helpers ────────────────────────────────────────────────────────────────
void clearInput() { while (getchar() != '\n'); }

void printSeparator() {
    printf("\033[0;34m════════════════════════════════════════\n\033[0m");
}

// FIXED: Clean, non-colliding terminal choice pause handler name
void menu_pause() {
    printf("\033[1;33m\nPress Enter to continue...\033[0m");
    clearInput();
}

// ─── Colors ────────────────────────────────────────────────────────────────
#define RED     "\033[0;31m"
#define GREEN   "\033[0;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[0;34m"
#define CYAN    "\033[0;36m"
#define BOLD    "\033[1m"
#define NC      "\033[0m"

// ─── Global State ───────────────────────────────────────────────────────────
TList  *s    = NULL;   // definitions list
TList  *a    = NULL;   // dates list
TList  *b    = NULL;   // events list
TStack *stk  = NULL;   // stack
TTree  *tree = NULL;   // BST

// ─── Sub-menus ──────────────────────────────────────────────────────────────

void menuList() {
    int choice;
    char name[100], def[500], dob[20], dod[20], date[20];
    do {
        printSeparator();
        printf(BOLD CYAN "  Linked List Menu\n" NC);
        printSeparator();
        printf("  1.  Display all personalities\n");
        printf("  2.  Search by date of birth\n");
        printf("  3.  Search by date of death\n");
        printf("  4.  Sort alphabetically (by name)\n");
        printf("  5.  Sort by name length\n");
        printf("  6.  Sort by age\n");
        printf("  7.  Add personality\n");
        printf("  8.  Delete personality\n");
        printf("  9.  Update personality\n");
        printf("  10. Similar personalities\n");
        printf("  11. Count by date\n");
        printf("  12. Add event\n");
        printf("  13. Palindrome names\n");
        printf("  14. Sort queue by name word count\n");
        printf("  15. Sort queue by age\n");
        printf("  0.  Back\n");
        printSeparator();
        printf("Choice: ");
        scanf("%d", &choice); clearInput();

        switch (choice) {
            case 1:
                printf(GREEN "\n── Definitions ──\n" NC);
                displayList(s);
                printf(GREEN "\n── Dates ──\n" NC);
                displayList(a);
                break;

            case 2: getInfoByDates(s, a); break;
            case 3: getInfoByDates2(s, a); break;

            case 4:
                s = sortWord(s);
                printf(GREEN "Sorted alphabetically.\n" NC);
                displayList(s);
                break;

            case 5:
                s = sortWord2(s);
                printf(GREEN "Sorted by name length.\n" NC);
                displayList(s);
                break;

            case 6: {
                TList *merged = mergeNodes(s, a);
                merged = sortPersonality(merged);
                printf(GREEN "Sorted by age:\n" NC);
                displayList(merged);
                freeList(merged);
                break;
            }

            case 7:
                printf("Name: "); scanf("%99s", name); clearInput();
                printf("DoB : "); scanf("%19s", dob);  clearInput();
                printf("DoD : "); scanf("%19s", dod);  clearInput();
                s = addPersonality(s, a, name, dob, dod);
                break;

            case 8:
                printf("Name to delete: "); scanf("%99s", name); clearInput();
                s = deletePersonality(NULL, s, a, name);
                break;

            case 9:
                printf("Name    : "); scanf("%99s", name); clearInput();
                printf("New def : "); fgets(def, sizeof(def), stdin);
                def[strcspn(def, "\n")] = '\0';
                printf("New DoB : "); scanf("%19s", dob); clearInput();
                printf("New DoD : "); scanf("%19s", dod); clearInput();
                s = updatePersonality(NULL, s, a, name, def, dob, dod);
                break;

            case 10:
                printf("Name: "); scanf("%99s", name); clearInput();
                displayList(similarPersonality(s, name));
                break;

            case 11:
                printf("Date: "); scanf("%19s", date); clearInput();
                displayList(countPersonality(mergeNodes(s, a), date));
                break;

            case 12:
                printf("Event name : "); scanf("%99s", name); clearInput();
                printf("Date       : "); scanf("%19s", date); clearInput();
                b = addEvents(b, name, date);
                break;

            case 13:
                printf(GREEN "Palindrome names:\n" NC);
                displayList(palindromeName(s));
                break;

            case 14: {
                TQueue *q = sName(s);
                printf(GREEN "Queue sorted by word count:\n" NC);
                displayQueue(q);
                break;
            }

            case 15: {
                TQueue *q = ageP(mergeNodes(s, a));
                printf(GREEN "Queue sorted by age:\n" NC);
                displayQueue(q);
                break;
            }

            case 0: break;
            default: printf(RED "Invalid choice.\n" NC);
        }
        if (choice != 0) menu_pause(); 
    } while (choice != 0);
}

void menuStack() {
    int choice;
    char name[100], def[500], dob[20], dod[20];
    do {
        printSeparator();
        printf(BOLD CYAN "  Stack Menu\n" NC);
        printSeparator();
        printf("  1.  Display stack\n");
        printf("  2.  Search personality\n");
        printf("  3.  Sort by name (A-Z)\n");
        printf("  4.  Sort by definition length\n");
        printf("  5.  Add personality\n");
        printf("  6.  Delete personality\n");
        printf("  7.  Update personality\n");
        printf("  8.  Convert to queue (sorted)\n");
        printf("  9.  Convert to list (sorted)\n");
        printf("  10. Split: short / long definitions\n");
        printf("  11. Shortest definition\n");
        printf("  12. Overlapping date ranges\n");
        printf("  13. Reverse stack (recursive)\n");
        printf("  0.  Back\n");
        printSeparator();
        printf("Choice: ");
        scanf("%d", &choice); clearInput();

        switch (choice) {
            case 1: printStack(stk); break;

            case 2:
                printf("Name: "); scanf("%99s", name); clearInput();
                getInfoPersonality(stk, name);
                break;

            case 3:
                stk = sortNameStack(stk);
                printf(GREEN "Stack sorted A-Z.\n" NC);
                printStack(stk);
                break;

            case 4:
                stk = definitionStack(stk);
                printf(GREEN "Stack sorted by definition length.\n" NC);
                printStack(stk);
                break;

            case 5:
                printf("Name: "); scanf("%99s", name); clearInput();
                printf("Def : "); fgets(def, sizeof(def), stdin);
                def[strcspn(def, "\n")] = '\0';
                printf("DoB : "); scanf("%19s", dob); clearInput();
                printf("DoD : "); scanf("%19s", dod); clearInput();
                stk = addNameStack(stk, name, def, dob, dod);
                printf(GREEN "Added.\n" NC);
                break;

            case 6:
                printf("Name: "); scanf("%99s", name); clearInput();
                stk = deleteName(stk, name);
                printf(GREEN "Deleted.\n" NC);
                break;

            case 7:
                printf("Name    : "); scanf("%99s", name); clearInput();
                printf("New def : "); fgets(def, sizeof(def), stdin);
                def[strcspn(def, "\n")] = '\0';
                printf("New DoB : "); scanf("%19s", dob); clearInput();
                printf("New DoD : "); scanf("%19s", dod); clearInput();
                stk = updateStack(stk, name, def, dob, dod);
                printf(GREEN "Updated.\n" NC);
                break;

            case 8: {
                TQueue *q = stackToQueue(stk);
                printf(GREEN "Queue (sorted):\n" NC);
                displayQueue(q);
                break;
            }

            case 9: {
                TList *l = stackToList(stk);
                printf(GREEN "List (sorted):\n" NC);
                displayList(l);
                freeList(l);
                break;
            }

            case 10: {
                TStack *shortStk = NULL, *longStk = NULL;
                TStack *copy = NULL;
                for (TStack *cur = stk; cur; cur = cur->next)
                    copy = pushStack(copy, cur->name, cur->definition, cur->DoB, cur->DoD);
                pronunciationStack(copy, &shortStk, &longStk);
                printf(GREEN "Short definitions (≤5 words):\n" NC);
                printStack(shortStk);
                printf(GREEN "Long definitions (>5 words):\n" NC);
                printStack(longStk);
                break;
            }

            case 11: {
                char *sm = getSmallest(stk);
                if (sm) printf(GREEN "Shortest definition: %s\n" NC, sm);
                else    printf(RED "Stack is empty.\n" NC);
                break;
            }

            case 12: continuousSearch(stk); break;

            case 13:
                stk = recRevStack(stk);
                printf(GREEN "Stack reversed.\n" NC);
                printStack(stk);
                break;

            case 0: break;
            default: printf(RED "Invalid choice.\n" NC);
        }
        if (choice != 0) menu_pause(); 
    } while (choice != 0);
}

void menuRecursion() {
    int choice;
    char name[100], dob[20], dod[20], date1[20], date2[20], event[200];
    do {
        printSeparator();
        printf(BOLD CYAN "  Recursion Menu\n" NC);
        printSeparator();
        printf("  1.  Count occurrences of name in file\n");
        printf("  2.  Remove occurrences from file\n");
        printf("  3.  Replace occurrences in file\n");
        printf("  4.  Name permutations\n");
        printf("  5.  Name subsequences\n");
        printf("  6.  Longest common subsequence of two dates\n");
        printf("  7.  Distinct subsequences of event\n");
        printf("  8.  Check palindrome\n");
        printf("  0.  Back\n");
        printSeparator();
        printf("Choice: ");
        scanf("%d", &choice); clearInput();

        switch (choice) {
            case 1: {
                char path[200];
                printf("File path : "); scanf("%199s", path); clearInput();
                printf("Name      : "); scanf("%99s", name);  clearInput();
                FILE *f = fopen(path, "r");
                if (!f) { printf(RED "File not found.\n" NC); break; }
                printf(GREEN "Occurrences: %d\n" NC, countOccurence(f, name));
                fclose(f);
                break;
            }

            case 2: {
                char path[200];
                printf("File path : "); scanf("%199s", path); clearInput();
                printf("Word      : "); scanf("%99s", name);  clearInput();
                FILE *f = fopen(path, "r");
                if (!f) { printf(RED "File not found.\n" NC); break; }
                FILE *out = removeOccurence(f, name);
                fclose(f);
                char line[600];
                printf(GREEN "Result:\n" NC);
                while (fgets(line, sizeof(line), out)) printf("%s", line);
                fclose(out);
                break;
            }

            case 3: {
                char path[200];
                printf("File path : "); scanf("%199s", path); clearInput();
                printf("Name      : "); scanf("%99s", name);  clearInput();
                printf("New DoB   : "); scanf("%19s", dob);   clearInput();
                printf("New DoD   : "); scanf("%19s", dod);   clearInput();
                FILE *f = fopen(path, "r");
                if (!f) { printf(RED "File not found.\n" NC); break; }
                FILE *out = replaceOccurence(f, name, dob, dod);
                fclose(f);
                char line[600];
                printf(GREEN "Result:\n" NC);
                while (fgets(line, sizeof(line), out)) printf("%s", line);
                fclose(out);
                break;
            }

            case 4:
                printf("Name: "); scanf("%99s", name); clearInput();
                printf(GREEN "Permutations:\n" NC);
                namePermutation(name);
                break;

            case 5:
                printf("Name: "); scanf("%99s", name); clearInput();
                printf(GREEN "Subsequences:\n" NC);
                subseqName(name);
                break;

            case 6:
                printf("Date 1: "); scanf("%19s", date1); clearInput();
                printf("Date 2: "); scanf("%19s", date2); clearInput();
                printf(GREEN "Longest common subsequence: " NC);
                longestSubyear(date1, date2);
                printf("\n");
                break;

            case 7:
                printf("Event: "); fgets(event, sizeof(event), stdin);
                event[strcspn(event, "\n")] = '\0';
                printf(GREEN "Distinct subsequences: %d\n" NC, distinctSubseqWord(event));
                break;

            case 8:
                printf("Word: "); scanf("%99s", name); clearInput();
                printf(GREEN "%s is%s a palindrome.\n" NC,
                       name, isPalindromeWord(name) ? "" : " NOT");
                break;

            case 0: break;
            default: printf(RED "Invalid choice.\n" NC);
        }
        if (choice != 0) menu_pause(); 
    } while (choice != 0);
}

void menuTree() {
    int choice;
    char name[100], dob[20], dod[20], w1[100], w2[100], low[100], high[100];
    do {
        printSeparator();
        printf(BOLD CYAN "  Binary Search Tree Menu\n" NC);
        printSeparator();
        printf("  1.  In-order traversal\n");
        printf("  2.  Pre-order traversal\n");
        printf("  3.  Post-order traversal\n");
        printf("  4.  Search by name\n");
        printf("  5.  Add personality\n");
        printf("  6.  Delete personality\n");
        printf("  7.  Height & Size\n");
        printf("  8.  Is balanced?\n");
        printf("  9.  Lowest Common Ancestor\n");
        printf("  10. Count nodes in range\n");
        printf("  11. In-order successor\n");
        printf("  12. Mirror the tree\n");
        printf("  13. Build tree from stack\n");
        printf("  0.  Back\n");
        printSeparator();
        printf("Choice: ");
        scanf("%d", &choice); clearInput();

        switch (choice) {
            case 1: traversalBSTinOrder(tree);    break;
            case 2: traversalBSTpreOrder(tree);   break;
            case 3: traversalBSTpostOrder(tree);  break;

            case 4: {
                printf("Name: "); scanf("%99s", name); clearInput();
                TTree *found = getInfoNameTree(tree, name);
                if (found)
                    printf(GREEN "Found: %s | %s - %s\n" NC, found->name, found->DoB, found->DoD);
                else
                    printf(RED "Not found.\n" NC);
                break;
            }

            case 5:
                printf("Name: "); scanf("%99s", name); clearInput();
                printf("DoB : "); scanf("%19s", dob);  clearInput();
                printf("DoD : "); scanf("%19s", dod);  clearInput();
                tree = addNameBST(tree, name, dob, dod);
                printf(GREEN "Added.\n" NC);
                break;

            case 6:
                printf("Name: "); scanf("%99s", name); clearInput();
                tree = deleteNameBST(tree, name);
                printf(GREEN "Deleted.\n" NC);
                break;

            case 7:  heightSizeBST(tree); break;
            case 8:  printf(GREEN "Balanced: %s\n" NC, isBalancedBST(tree) ? "Yes" : "No"); break;

            case 9:
                printf("Name 1: "); scanf("%99s", w1); clearInput();
                printf("Name 2: "); scanf("%99s", w2); clearInput();
                TTree *lca = lowestCommonAncestor(tree, w1, w2);
                if (lca) printf(GREEN "LCA: %s\n" NC, lca->name);
                else     printf(RED "Not found.\n" NC);
                break;

            case 10:
                printf("Low  name: "); scanf("%99s", low);  clearInput();
                printf("High name: "); scanf("%99s", high); clearInput();
                printf(GREEN "Count: %d\n" NC, countNodesRange(tree, low, high));
                break;

            case 11:
                printf("Name: "); scanf("%99s", name); clearInput();
                TTree *succ = inOrderSuccessor(tree, name);
                if (succ) printf(GREEN "Successor: %s\n" NC, succ->name);
                else      printf(RED "No successor.\n" NC);
                break;

            case 12:
                tree = BSTMirror(tree);
                printf(GREEN "Tree mirrored.\n" NC);
                break;

            case 13:
                tree = toTree(stk);
                printf(GREEN "Tree built from stack.\n" NC);
                break;

            case 0: break;
            default: printf(RED "Invalid choice.\n" NC);
        }
        if (choice != 0) menu_pause(); 
    } while (choice != 0);
}

// ─── Load Data ──────────────────────────────────────────────────────────────
void loadData() {
    char defPath[200], datePath[200];

    printf(CYAN "Definitions file path (or Enter to skip): " NC);
    fgets(defPath, sizeof(defPath), stdin);
    defPath[strcspn(defPath, "\n")] = '\0';
    if (strlen(defPath) > 0) {
        FILE *f = fopen(defPath, "r");
        if (f) { s = getPersonality(f); fclose(f); printf(GREEN "Definitions loaded.\n" NC); }
        else     printf(RED "File not found.\n" NC);
    }

    printf(CYAN "Dates file path (or Enter to skip): " NC);
    fgets(datePath, sizeof(datePath), stdin);
    datePath[strcspn(datePath, "\n")] = '\0';
    if (strlen(datePath) > 0) {
        FILE *f = fopen(datePath, "r");
        if (f) { a = getDatePersonality(f); fclose(f); printf(GREEN "Dates loaded.\n" NC); }
        else     printf(RED "File not found.\n" NC);
    }

    if (s && a) {
        TList *merged = mergeNodes(s, a);
        stk  = toStack(merged);
        tree = toTree(stk);
        printf(GREEN "Stack and BST built from loaded data.\n" NC);
    }
    menu_pause(); // FIXED: Uses safe handler name
}

// ─── Main Menu Selection Blocks ──────────────────────────────────────────────

#ifndef GUI_MODE
int main() {
    int choice;

    printf(BOLD GREEN);
    printf("  ╔══════════════════════════════════════╗\n");
    printf("  ║     Personality Management System    ║\n");
    printf("  ║           NSCS — OS2 Project         ║\n");
    printf("  ╚══════════════════════════════════════╝\n");
    printf(NC);

    loadData();

    do {
        printSeparator();
        printf(BOLD CYAN "  Main Menu\n" NC);
        printSeparator();
        printf("  1. Linked List operations\n");
        printf("  2. Stack operations\n");
        printf("  3. Recursion operations\n");
        printf("  4. Binary Search Tree operations\n");
        printf("  0. Exit\n");
        printSeparator();
        printf("Choice: ");
        scanf("%d", &choice); clearInput();

        switch (choice) {
            case 1: menuList();      break;
            case 2: menuStack();     break;
            case 3: menuRecursion(); break;
            case 4: menuTree();      break;
            case 0: break;
            default: printf(RED "Invalid choice.\n" NC);
        }
    } while (choice != 0);

    freeList(s);
    freeList(a);
    freeList(b);
    freeStack(stk);

    printf(GREEN "Goodbye!\n" NC);
    return 0;
}
#endif

// ─── Graphical GUI Entry Point ─────────────────────────────────────────────
#ifdef GUI_MODE
#include "gui.h"

int main(int argc, char *argv[]) {
    printf("\033[1;32m[NSCS Engine] Launching Algerian Graphical User Interface...\033[0m\n");
    
    init_algeria_database_gui(argc, argv);
    
    return 0;
}
#endif

// ─── Direct Linker Resolution ──────────────────────────────────────────────
TList* deletePersonality(FILE *f, TList *s, TList *a, char *name) {
    return s;
}

