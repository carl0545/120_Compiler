#ifndef INTERMEDIATE_H
#define INTERMEDIATE_H

#include "tac.h"
#include "tree.h"
#include "symtable.h"

//void codegen(struct tree *parseT);

int ht_set_size(struct hashtable_s *hashtable, int *local, int *global, int *parameter, enum regions_h currRegion); //Assigns the size attribute to every sym table entry

void setParam(struct listnode *head, int *global, int *local, int *param, int *total, enum regions_h currRegion);

int getOffset(enum regions_h currRegion, int *global, int *local, int *param, int increment, int *total);
void printSize(struct hashtable_s *hashtable);
void printParam(struct listnode *head, struct entry_s *entry_v);
int getArrayType(int baseT);
void arrayOffset(enum regions_h currRegion, int *global, int *local, int *param, int increment, int size_a, int *total);





#endif
