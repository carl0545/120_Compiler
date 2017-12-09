#ifndef INTERMEDIATE_H
#define INTERMEDIATE_H

#include "tac.h"
#include "tree.h"
#include "symtable.h"
#include <stdio.h>




int label_counter;
int global_r_counter;
int local_r_counter;
int param_r_counter;
int float_r_counter;
struct hashtable_s *i_scope;

void init_imd_globals();

int ht_set_size(struct hashtable_s *hashtable, int *local, int *global, int *parameter, enum regions_h currRegion); //Assigns the size attribute to every sym table entry

void setParam(struct listnode *head, int *global, int *local, int *param, int *total, enum regions_h currRegion);

int getOffset(enum regions_h currRegion, int *global, int *local, int *param, int increment, int *total);
void printSize(struct hashtable_s *hashtable);
void printParam(struct listnode *head, struct entry_s *entry_v);
int getArrayType(int baseT);
void arrayOffset(enum regions_h currRegion, int *global, int *local, int *param, int increment, int size_a, int *total);


struct addr* addr_create(int reg, int offs);
struct addr* label_create();
struct addr* temp_create();

void codeGen(struct tree *parseT);
void gen_first(struct tree *parseT);
void gen_follows(struct tree *parseT);
void gen_temp(struct tree *parseT);
void gen_place(struct tree *parseT);

void place_helper(struct tree *parseT);

void print_main(struct instr *head, FILE *fpi);
void print_instr(struct instr *head, FILE *fpi);
void print_region_helper(FILE *fpi, int reg, int off);



#endif
