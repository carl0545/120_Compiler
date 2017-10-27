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

    ;
  }


}

void type_assign_exp(struct tree *parseT){
    struct type120 *t1, *t2;


}

void type_compare(struct type120 *type1, struct type120 *type2){


}

void scope_change(struct tree *parseT){
  int productRule = parseT->prodrule;

 //need to implement this later
 printf("NEXT TO IMPLEMENT\n");
  exit(3);
  switch(productRule){

  }

}

void init_type(){
  currScope = global;

  if(currScope == NULL){
    fprintf(stderr, "%s\n", "Type.c: init_type: Error with global scope reporting null");
    exit(3);
  }

}
