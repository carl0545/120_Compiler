/*
*Altered code from https://gist.github.com/tonious/1377667
*/

#ifndef HASH_H
#define HASH_H


#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "symtable.h"

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





#endif
