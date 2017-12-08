#ifndef TREE_H
#define TREE_H

#include "token.h"
#include "tac.h"

struct tree{
  int leafCheck; // 0 for not leaf 1 for leaf
  int prodrule;
  int nkids;
  struct tree *kids[10];
  token *leaf;

};

struct tree *createTreeNode(int prodrule, int nkids, ...);
int treeprint(struct tree *t, int depth);
char *humanreadable(int prod);

#endif
