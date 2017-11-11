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
void type_unary_express(struct tree*); //handles unary_expression product rule
void type_init_declarator(struct tree*); //handles init_declarator product rule
void type_array_check(struct tree*); //check an single array assignment
void type_postfix_exp_2(struct tree*); //handle postfix_expression-2
void type_mult_assign_exp(struct tree*);//handles assignment_expression product rule with multiple right hand side assignments
void type_relational_express(struct tree*);//handes relational_expression product rule
struct type120* relation_helper(struct tree*); //helper for relational_expression
void mult_helper(struct tree*, struct type120*); //helper function for type_mult_assign_exp
void mult_postfix_helper(struct tree*, struct type120*, int); //helper function for type_mult_assign_exp for postfix_expressions

bool arr_check(struct tree*);//check if in an array
void type_compare(int operand, struct type120*, struct type120*); //checks to see if two types are the same

#endif
