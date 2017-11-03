#include "type.h"
#include "nonterms.h"
#include "120gram.h"
#include "symtable.h"


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

      if(parseT->kids[2]->prodrule > 0){
        printf("Single assignment\n");
        type_assign_exp(parseT);
      }
      else if(parseT->kids[2]->prodrule == postfix_expression){
        printf("I'm in a postfix_expression\n");
        type_postfix_exp(parseT);
      }
      else{
        printf("multiple assignment\n");
        type_mult_assign_exp(parseT);
      }
  }


}

void type_postfix_exp(struct tree *parseT){
  struct type120 *left, *right;

  left = ht_get(currScope, parseT->kids[0]->leaf->text);
  right = ht_get(currScope, parseT->kids[2]->kids[0]->leaf->text);

  type_compare(-1, left, right);
}

void type_mult_assign_exp(struct tree *parseT){
  struct type120 *left;

  left = ht_get(currScope, parseT->kids[0]->leaf->text);

  mult_helper(parseT->kids[2], left);

}

void mult_helper(struct tree *parseT, struct type120 *left){
  int count;
  struct tree *iter;

  count = 0;
  iter = parseT;
  while(iter->prodrule < 0){
    count++;
    iter = iter->kids[0];
  }


  for(int i = 0; i < count; i++){
    iter = parseT;

    for(int k = i; k < count-1; k++){
      iter = iter->kids[0];
    }


    if(i == 0){
      type_compare(iter->kids[1]->prodrule, left, ht_get(currScope, iter->kids[0]->leaf->text));
    }

    if(iter->kids[2]->prodrule > 0){
      type_compare(iter->kids[1]->prodrule, left, ht_get(currScope, iter->kids[2]->leaf->text));
    }
    else{
      mult_helper(parseT->kids[2], left);
    }

  }

}

void type_assign_exp(struct tree *parseT){
    struct type120 *left, *right;


    left = ht_get(currScope, parseT->kids[0]->leaf->text);
    right = ht_get(currScope, parseT->kids[2]->leaf->text);

    type_compare(-1, left, right);
}

void type_compare(int operand, struct type120 *type1, struct type120 *type2){
    enum types leftT, rightT;

    leftT = type1->base_type;
    rightT = type2->base_type;

    if(rightT == FUNCTION_T){
      printf("We have a function return type\n");
      rightT = type2->u.function.elemtype->base_type;
    }

    if(leftT != rightT){
      fprintf(stderr, "Type Error: variable assignment doesn't matched declaration\n");
      exit(3);
    }


    switch(operand){
      case -1: //no operand, simple assignment
        return;
      case MINUS:
        if(leftT == INT_T || leftT == DOUBLE_T){

        }
        else{
          fprintf(stderr, "Type Error: Operator/Operand error\n");
          fprintf(stderr, "Operator: %d Operand: %d\n", leftT, operand);
          exit(3);
        }
      case PLUS:
        if(leftT == INT_T || leftT == DOUBLE_T){

        }
        else{
          fprintf(stderr, "Type Error: Operator/Operand error\n");
          fprintf(stderr, "Operator: %d Operand: %d\n", leftT, operand);
          exit(3);
        }
      case MUL:
        if(leftT == INT_T || leftT == DOUBLE_T){

        }
        else{
          fprintf(stderr, "Type Error: Operator/Operand error\n");
          fprintf(stderr, "Operator: %d Operand: %d\n", leftT, operand);
          exit(3);
        }
      case DIV:
        if(leftT == INT_T || leftT == DOUBLE_T){

        }
        else{
          fprintf(stderr, "Type Error: Operator/Operand error\n");
          fprintf(stderr, "Operator: %d Operand: %d\n", leftT, operand);
          exit(3);
        }
      case MOD:
        if(leftT == INT_T || leftT == DOUBLE_T){

        }
        else{
          fprintf(stderr, "Type Error: Operator/Operand error\n");
          fprintf(stderr, "Operator: %d Operand: %d\n", leftT, operand);
          exit(3);
        }
    }

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
