/*
*Altered code from https://gist.github.com/tonious/1377667
*/

#ifndef HASH_H
#define HASH_H


#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "symtable.h"

#define INT_SIZE 8
#define DOUBLE_SIZE 8
#define CHAR_SIZE 1
#define STRING_SIZE 8


enum regions_h {
	  GLOBAL_H,
		LOCAL_H,
		PARAM_H
} regions_h;

struct entry_s {
	char *key;
	struct type120 *value;
	struct entry_s *next;
};


struct hashtable_s {
	struct hashtable_s *parent;
	int size;
	struct entry_s **table;
};


struct hashtable_s* ht_create(int size, struct hashtable_s *par);//Create new hashtable of a size
int ht_hash(struct hashtable_s *hashtable, char *key); //Hash a string for a particular hash table
struct entry_s *ht_newpair(char *key, struct type120 *value);//Create a new entry node
void ht_set(struct hashtable_s *hashtable, char *key, struct type120 *value);//Insert an entry into hash table
struct type120 *ht_get(struct hashtable_s *hashtable, char *key);//Retrieve a node

int ht_set_size(struct hashtable_s *hashtable, int *local, int *global, int *parameter, enum regions_h currRegion); //Assigns the size attribute to every sym table entry

int getOffset(enum regions_h currRegion, int *global, int *local, int *param, int increment, int *total);
void printSize(struct hashtable_s *hashtable);
int getArrayType(int baseT);
void arrayOffset(enum regions_h currRegion, int *global, int *local, int *param, int increment, int size_a, int *total);



#endif
