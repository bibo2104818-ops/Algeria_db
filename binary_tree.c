#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "binary_tree.h"

// helper functions
// allocate a new tree node
TTree* createTreeNode(char *name, char *DoB, char *DoD) {
    TTree *newNode = (TTree*)malloc(sizeof(TTree));
    if (newNode) {
        strcpy(newNode->name, name);
        strcpy(newNode->DoB, DoB);
        strcpy(newNode->DoD, DoD);
        newNode->left = newNode->right = NULL;
    }
    return newNode;
}

//recursive helper for insertion
TTree* insertBST(TTree *root, char *name, char *DoB, char *DoD) {
    if (root == NULL) return createTreeNode(name, DoB, DoD);
    
    // Names are sorted alphabetically using strcmp [cite: 75]
    if (strcmp(name, root->name) < 0)
        root->left = insertBST(root->left, name, DoB, DoD);
    else if (strcmp(name, root->name) > 0)
        root->right = insertBST(root->right, name, DoB, DoD);
        
    return root;
}

//find the node with the minimum value (used for deletion)
TTree* findMin(TTree* root) {
    while (root->left != NULL) root = root->left;
    return root;
}
TTree* fillTree(FILE *f) {
    char line[600];
    TTree *root = NULL;
    while (fgets(line, sizeof(line), f)) {
        char name[100], DoB[20], DoD[20];
        if (sscanf(line, "%99[^=]=%*[^%]{%19[^-]-%19[^}]}", name, DoB, DoD) == 3) {
            root = insertBST(root, name, DoB, DoD);
        }
    }
    return root;
}

TTree* getInfoNameTree(TTree *tr, char *name) {
    if (tr == NULL || strcmp(tr->name, name) == 0) return tr;

    if (strcmp(name, tr->name) < 0)
        return getInfoNameTree(tr->left, name);
    
    return getInfoNameTree(tr->right, name);
}
TTree* deleteNameBST(TTree *tr, char *name) {
    if (tr == NULL) return tr;

    if (strcmp(name, tr->name) < 0)
        tr->left = deleteNameBST(tr->left, name);
    else if (strcmp(name, tr->name) > 0)
        tr->right = deleteNameBST(tr->right, name);
    else {
        // Node with only one child or no child
        if (tr->left == NULL) {
            TTree *temp = tr->right;
            free(tr);
            return temp;
        } else if (tr->right == NULL) {
            TTree *temp = tr->left;
            free(tr);
            return temp;
        }
        // Node with two children: Get the in-order successor
        TTree *temp = findMin(tr->right);
        strcpy(tr->name, temp->name);
        tr->right = deleteNameBST(tr->right, temp->name);
    }
    return tr;
}
void heightSizeBST(TTree *tr) {
    
    printf("Height: %d, Size: %d\n", getTreeHeight(tr), getTreeSize(tr));
}

bool isBalancedBST(TTree *tr) {
    if (tr == NULL) return true;
    int lh = getTreeHeight(tr->left);
    int rh = getTreeHeight(tr->right);
    if (abs(lh - rh) <= 1 && isBalancedBST(tr->left) && isBalancedBST(tr->right))
        return true;
    return false; 
}
TTree* toTree(TStack *stk) {
    TTree *tr = NULL;
    TStack *temp = stk;
    while (temp != NULL) {
        tr = insertBST(tr, temp->name, temp->DoB, temp->DoD);
        temp = temp->next;
    }
    return tr;
}

TTree* addNameBST(TTree *tr, char *name, char *DoB, char *DoD) {
    return insertBST(tr, name, DoB, DoD);
}

void traversalBSTinOrder(TTree *tr) {
    if (tr != NULL) {
        traversalBSTinOrder(tr->left);
        printf("Name: %s | Dates: %s - %s\n", tr->name, tr->DoB, tr->DoD);
        traversalBSTinOrder(tr->right);
    }
}

void traversalBSTpreOrder(TTree *tr) {
    if (tr != NULL) {
        printf("Name: %s | Dates: %s - %s\n", tr->name, tr->DoB, tr->DoD);
        traversalBSTpreOrder(tr->left);
        traversalBSTpreOrder(tr->right);
    }
}

void traversalBSTpostOrder(TTree *tr) {
    if (tr != NULL) {
        traversalBSTpostOrder(tr->left);
        traversalBSTpostOrder(tr->right);
        printf("Name: %s | Dates: %s - %s\n", tr->name, tr->DoB, tr->DoD);
    }
}

TTree* lowestCommonAncestor(TTree *tr, char *word1, char *word2) {
    if (tr == NULL) return NULL;

    if (strcmp(word1, tr->name) < 0 && strcmp(word2, tr->name) < 0)
        return lowestCommonAncestor(tr->left, word1, word2);

    if (strcmp(word1, tr->name) > 0 && strcmp(word2, tr->name) > 0)
        return lowestCommonAncestor(tr->right, word1, word2);

    return tr;
}

int countNodesRange(TTree *tr, char *low, char *high) {
    if (tr == NULL) return 0;

    if (strcmp(tr->name, low) >= 0 && strcmp(tr->name, high) <= 0)
        return 1 + countNodesRange(tr->left, low, high) + countNodesRange(tr->right, low, high);

    else if (strcmp(tr->name, low) < 0)
        return countNodesRange(tr->right, low, high);

    else
        return countNodesRange(tr->left, low, high);
}

TTree* inOrderSuccessor(TTree *tr, char *word) {
    TTree *current = getInfoNameTree(tr, word);
    if (current == NULL) return NULL;

    if (current->right != NULL) {
        TTree *temp = current->right;
        while (temp->left != NULL) temp = temp->left;
        return temp;
    }

    TTree *successor = NULL;
    TTree *ancestor = tr;
    while (ancestor != current) {
        if (strcmp(current->name, ancestor->name) < 0) {
            successor = ancestor;
            ancestor = ancestor->left;
        } else {
            ancestor = ancestor->right;
        }
    }
    return successor;
}
// Helper to flatten the BST into an ordered array
void treeToArray(TTree *root, TTree **arr, int *index) {
    if (root == NULL) return;
    treeToArray(root->left, arr, index);
    arr[(*index)++] = root;
    treeToArray(root->right, arr, index);
}

// Helper to build a completely balanced BST from the flattened array
TTree* buildBalancedBST(TTree **arr, int start, int end) {
    if (start > end) return NULL;
    
    int mid = (start + end) / 2;
    TTree *root = arr[mid];
    
    root->left = buildBalancedBST(arr, start, mid - 1);
    root->right = buildBalancedBST(arr, mid + 1, end);
    
    return root;
}
TTree* BSTMirror(TTree *tr) {
    if (tr == NULL) return NULL;

    TTree *temp = tr->left;
    tr->left = BSTMirror(tr->right);
    tr->right = BSTMirror(temp);

    return tr;
}

TTree* BTSMerge(TTree *tr1, TTree *tr2) {
    int size1 = getTreeSize(tr1);
    int size2 = getTreeSize(tr2);
    TTree **arr = malloc(sizeof(TTree*) * (size1 + size2));
    
    int index = 0;
    treeToArray(tr1, arr, &index);
    treeToArray(tr2, arr, &index);
    
    return buildBalancedBST(arr, 0, index - 1);
}
int getTreeHeight(TTree *tr) {
    if (!tr) return 0;
    int l = getTreeHeight(tr->left);
    int r = getTreeHeight(tr->right);
    return 1 + (l > r ? l : r);
}

int getTreeSize(TTree *tr) {
    if (!tr) return 0;
    return 1 + getTreeSize(tr->left) + getTreeSize(tr->right);
}