#include "type.h"
#include "nonterms.h"
#include "120gram.h"


void type_check(struct tree *parseT){
  if(parseT == NULL){
    return;
  }

  int productRule = parseT->prodrule;

  switch(productRule){
    case function_definition-1:
      printf("%s\n", "I'm in a function_definition");
      scope_change(parseT);
    case expression_statement:
      printf("I'm in an expression statement\n");
      type_express_state(parseT->kids[0]);

  }

  //iterate through the rest of the tree
  for(int i = 0; i < parseT->nkids; i++){
    type_check(parseT->kids[i]);
  }

}

void type_express_state(struct tree *parseT){
  int productRule = parseT->prodrule;

  switch(productRule){
    case assignment_expression:
      printf("Entering assignment_expression\n");
      type_assign_exp(parseT);
  }


}

void type_assign_exp(struct tree *parseT){
    struct type120 *left, *right;
    int leftT, rightT;

    left = ht_get(currScope, parseT->kids[0]->leaf->text);
    right = ht_get(currScope, parseT->kids[2]->leaf->text);

    leftT = left->base_type;
    rightT = right->base_type;

    if(rightT == FUNCTION_T){
      printf("We have a function return type\n");
      rightT = right->u.function.elemtype->base_type;
    }

    if(leftT != rightT){
      fprintf(stderr, "Type Error: variable assignment doesn't matched declaration\n");
      exit(3);
    }
}

void type_compare(struct type120 *type1, struct type120 *type2){


}

void scope_change(struct tree *parseT){
  struct hashtable_s* newScope;
  int productRule;

  productRule = parseT->kids[1]->prodrule;


  switch(productRule){
    case declarator:
      printf("I'm in a declarator statement\n");
      newScope = ht_get(currScope, parseT->kids[1]->kids[1]->kids[0]->leaf->text)->u.function.sources;
    case direct_declarator:
      printf("I'm in a direct_declarator statement\n");
      newScope = ht_get(currScope, parseT->kids[1]->kids[0]->leaf->text)->u.function.sources;
  }

  currScope = newScope;

}

void init_type(){
  currScope = global;

  if(currScope == NULL){
    fprintf(stderr, "%s\n", "Type.c: init_type: Error with global scope reporting null");
    exit(3);
  }

}
