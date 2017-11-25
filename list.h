#ifndef LIST_H
#define LIST_H

#include "symtable.h"
#include<stdio.h>
#include<stdlib.h>

struct listnode{
  struct type120 *type;
  struct listnode *next;
};

void add(struct listnode**, struct type120*);
void printList(struct listnode*);
int listSize(struct listnode*);
struct type120* listGet(struct listnode*, int elem);


#endif
