#ifndef TYPE_H
#define TYPE_H

#include <stdlib.h>
#include <stdio.h>
#include "tree.h"
#include "hash.h"
#include "symtable.h"


struct hashtable_s *currScope;

void init_type(); //initalizes neccesary variables necessary prior to typechecking

void scope_change(struct tree*);//changes the scope

void type_check(struct tree*); //main recursive call - performs a type check on all expressions
void type_express_state(struct tree*);// handles expression_statement product rule
void type_assign_exp(struct tree*);// handles assignment_expression product rule with only one assignment
void type_postfix_exp(struct tree*);// handles postfix_expression product rule
void type_mult_assign_exp(struct tree*);//handles assignment_expression product rule with multiple right hand side assignments
void mult_helper(struct tree*, struct type120*); //helper function for type_mult_assign_exp

void type_compare(int operand, struct type120*, struct type120*); //checks to see if two types are the same

#endif
