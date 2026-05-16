#ifndef BINARY_TREE_H
#define BINARY_TREE_H
#include <stdio.h>
#include <stdbool.h>
#include "types.h"

TTree* createTreeNode(char *name, char *DoB, char *DoD);
TTree* insertBST(TTree *root, char *name, char *DoB, char *DoD);
TTree* fillTree(FILE *f);
TTree* getInfoNameTree(TTree *tr, char *name);
TTree* deleteNameBST(TTree *tr, char *name);
TTree* addNameBST(TTree *tr, char *name, char *DoB, char *DoD);
TTree* toTree(TStack *stk);
TTree* lowestCommonAncestor(TTree *tr, char *word1, char *word2);
TTree* inOrderSuccessor(TTree *tr, char *word);
TTree* BSTMirror(TTree *tr);
void treeToArray(TTree *root, TTree **arr, int *index);
TTree* buildBalancedBST(TTree **arr, int start, int end);
TTree* BTSMerge(TTree *tr1, TTree *tr2);
TTree* findMin(TTree *root);
void   traversalBSTinOrder(TTree *tr);
void   traversalBSTpreOrder(TTree *tr);
void   traversalBSTpostOrder(TTree *tr);
void   heightSizeBST(TTree *tr);
int    getTreeHeight(TTree *tr);
int    getTreeSize(TTree *tr);
int    countNodesRange(TTree *tr, char *low, char *high);
bool   isBalancedBST(TTree *tr);

#endif
