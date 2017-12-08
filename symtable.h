#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <stdbool.h>
#include "hash.h"
#include "tree.h"
#include "list.h"
#include "tac.h"


enum types {
  INT_T,
  DOUBLE_T,
  CHAR_T,
  BOOL_T,
  ARRAY_T,
  FUNCTION_T,
  CLASS_T,
  VOID_T,
  UNKNOWN_T
} types;


struct type120 {
   enum types base_type; //Contains Type name
   bool pointer; //true if pointer false if not
   struct addr place; //hold region/offset
   union {
      struct arrayInfo {
         int size; /* allow for missing size, e.g. -1 */
	       struct type120 *elemtype;
      } array;
      struct functionInfo {
         struct type120 *elemtype; //return type
         struct listnode *parameters; //struct list needs to be created
         struct hashtable_s *sources;
      } function;
      struct classInfo {
         char *type;
         struct hashtable_s *private;
         struct hashtable_s *public;
      } class;
   } u;
};

static const int ROOTSIZE = 100;
static const int FUNCTIONSIZE = 100;
static const int CLASSSIZE = 100;

struct hashtable_s *curr;
struct hashtable_s *global;
bool mainCheck;

extern int typenametable_lookup(char*);

void populateSymbolTable(struct tree*);
void initGlobal();
void handle_init_decl(struct tree*);
void handle_func_decl(struct tree*, struct type120*, bool, bool, bool);
void handle_arr_decl(struct tree*, struct type120*);
void handle_func_def(struct tree*);
void handle_class_spec(struct tree*);
void handle_member_spec1(struct tree*, struct type120*);
void handle_member_spec2(struct tree*, struct type120*);
void handle_c_func_decl(struct tree*);
void handle_c_func_def(struct tree*, bool);
void handle_c_func_decl_inner(struct tree*);
void handle_c_func_def_inner(struct tree*);
void handle_c_func_def_constr(struct tree*);
void handle_c_func_decl_constr(struct tree*);
void handle_init_list(struct tree*);
void handle_expr_state(struct tree*);
void handle_literal(int, struct tree*);
void handle_identfier(struct tree*);
bool checkMain(struct tree*);
int find_base_type(int);
void class_check(struct type120*, struct tree*);
bool checkParams(struct listnode **p1, struct listnode **p2);


#endif
