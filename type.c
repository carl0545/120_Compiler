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

      scope_change(parseT);
      break;
    case expression_statement:

      type_express_state(parseT->kids[0]);
      break;
    case relational_expression:
      type_relational_express(parseT);
      break;
    case unary_expression:
      type_unary_express(parseT);
      break;
    case init_declarator:
      type_init_declarator(parseT);

  }

  //iterate through the rest of the tree
  for(int i = 0; i < parseT->nkids; i++){
    type_check(parseT->kids[i]);
  }

}

bool arr_check(struct tree *parseT){
  if(parseT->kids[0] == NULL){
    return false;
  }
  if(parseT->kids[0]->prodrule == direct_declarator || parseT->kids[0]->prodrule == direct_declarator-1){

    return true;
  }
  else{

    return false;
  }
}

void type_init_declarator(struct tree *parseT){
  //check if array declaration
  //arr_check(parseT);


  struct type120 *left, *right;

  if(parseT->kids[1] == NULL){
    return;
  }

  if(parseT->kids[1]->prodrule != initializer){
    return;
  }




  left = ht_get(currScope, parseT->kids[0]->leaf->text);

  if(parseT->kids[1]->kids[1]->prodrule == postfix_expression){
    if(parseT->kids[1]->kids[1]->kids[0]->prodrule == postfix_expression-2){
      struct type120 *class_v, *member_v;
      struct hashtable_s *tempScope;
      int arrFlag = 0;

      if(parseT->kids[1]->kids[1]->kids[0]->kids[0]->prodrule == postfix_expression){
        arrFlag = 1;
      }


      if(arrFlag == 1){
        class_v = ht_get(currScope, parseT->kids[1]->kids[1]->kids[0]->kids[0]->kids[0]->leaf->text);
      }
      else{
        class_v = ht_get(currScope, parseT->kids[1]->kids[1]->kids[0]->kids[0]->leaf->text);
      }

      if(arrFlag == 1){
        tempScope = class_v->u.array.elemtype->u.class.private;
      }
      else{
        tempScope = class_v->u.class.private;
      }

      member_v = ht_get(tempScope, parseT->kids[1]->kids[1]->kids[0]->kids[2]->leaf->text);
      right = member_v->u.function.elemtype;

    }
    else{
      right = ht_get(currScope, parseT->kids[1]->kids[1]->kids[0]->leaf->text);
    }
  }
  else{
    right = ht_get(currScope, parseT->kids[1]->kids[1]->leaf->text);
  }

  type_compare(-1, left, right);
}

void type_unary_express(struct tree *parseT){
  if(parseT->kids[1]->prodrule == primary_expression){
    return;
  }
  else {
    fprintf(stderr, "TYPE ERROR: Not (!) operator error\n");
    exit(3);
  }
}

void type_relational_express(struct tree *parseT){
  struct type120 *left, *right;
  int leftCheck = 0;
  int rightCheck = 0;


  if(parseT->kids[0]->prodrule == postfix_expression){
    left = relation_helper(parseT->kids[0]);
    leftCheck = 1;
  }

  if(parseT->kids[2]->prodrule == postfix_expression){
    right = relation_helper(parseT->kids[2]);
    rightCheck = 1;
  }

  if(leftCheck == 0){
    left = ht_get(currScope, parseT->kids[0]->leaf->text);
  }

  if(rightCheck == 0){
    right = ht_get(currScope, parseT->kids[2]->leaf->text);
  }


  type_compare(parseT->kids[1]->prodrule, left, right);

}

struct type120* relation_helper(struct tree *parseT){
  struct type120 *class_v, *member_v;
  struct hashtable_s *tempScope;
  int arrFlag = 0;

  if(parseT->kids[0]->kids[0]->prodrule == postfix_expression){
    arrFlag = 1;
  }

  if(arrFlag == 1){
    class_v = ht_get(currScope, parseT->kids[0]->kids[0]->kids[0]->leaf->text);
  }
  else{
    class_v = ht_get(currScope, parseT->kids[0]->kids[0]->leaf->text);
  }

  if(arrFlag == 1){
    tempScope = class_v->u.array.elemtype->u.class.private;
  }
  else{
    tempScope = class_v->u.class.private;
  }

  member_v = ht_get(tempScope, parseT->kids[0]->kids[2]->leaf->text);

  return member_v;
}

void type_array_check(struct tree *parseT){
  struct type120 *arr, *left, *right;

  arr = ht_get(currScope, parseT->kids[0]->kids[0]->leaf->text);

  //array size check

  if(arr->u.array.size <= parseT->kids[0]->kids[2]->leaf->ival){
    fprintf(stderr, "Array assignment out of bounds\n");
    exit(3);
  }


  left = arr->u.array.elemtype;
  right = ht_get(currScope, parseT->kids[2]->leaf->text);

  type_compare(-1, left, right);
}

void type_express_state(struct tree *parseT){
  int productRule = parseT->prodrule;

  switch(productRule){
    case assignment_expression:


      if(parseT->kids[0]->prodrule == postfix_expression){

        type_array_check(parseT);
      }
      else if(parseT->kids[2]->prodrule > 0){

        type_assign_exp(parseT);
      }
      else if(parseT->kids[2]->prodrule == postfix_expression){
        if(parseT->kids[2]->kids[0]->prodrule == postfix_expression-2){
        //  if(parseT->kids[2]->kids[0]->kids[0]->prodrule == postfix_expression){
        //    printf("I'm in the array postfix_expression\n");
        //    type_postfix_exp_3(parseT);
        //  }
        //  else{

            type_postfix_exp_2(parseT);
        //  }
        }
        else{

        type_postfix_exp(parseT);
        }
      }
      else{

        type_mult_assign_exp(parseT);
      }
      break;
    case postfix_expression:
    //  if(parseT->kids[0]->prodrule == postfix_expression-2){
    //    type_postfix_exp_2(parseT->kids[0]);
    //  }
      break;
    case shift_expression:
      type_shift_exp(parseT);
      break;
  }

}

void shift_post_helper_2(struct tree *parseT){
  struct type120 *class_v, *member_v, *right;
  struct hashtable_s *tempScope;
  int arrFlag = 0;

  if(parseT->kids[0]->prodrule == postfix_expression){
    arrFlag = 1;
  }

  if(arrFlag == 1){
    class_v = ht_get(currScope, parseT->kids[0]->kids[0]->leaf->text);
  }
  else{
    class_v = ht_get(currScope, parseT->kids[0]->leaf->text);
  }

  if(arrFlag == 1){
    tempScope = class_v->u.array.elemtype->u.class.private;
  }
  else{
    tempScope = class_v->u.class.private;
  }

  member_v = ht_get(tempScope, parseT->kids[2]->leaf->text);

  right = member_v->u.function.elemtype;

  switch(right->base_type){
    case INT_T:
      return;
    case DOUBLE_T:
      return;
    case CHAR_T:
      return;
    case CLASS_T:
      //fix later to check if string
      return;
    default:
      fprintf(stderr, "Shift expression postfix error\n" );
      exit(3);
  }
}

void shift_post_helper(struct tree *parseT){
  //check if in a class postfix
  if(parseT->kids[0]->prodrule == postfix_expression-2){
    shift_post_helper_2(parseT->kids[0]);
    return;
  }

  struct type120 *func_v, *return_v;

  func_v = ht_get(currScope, parseT->kids[0]->leaf->text);

  return_v = func_v->u.function.elemtype;

  switch(return_v->base_type){
    case INT_T:
      return;
    case DOUBLE_T:
      return;
    case CHAR_T:
      return;
    case CLASS_T:
      //fix later to check if string
      return;
    default:
      fprintf(stderr, "Shift expression postfix error\n" );
      exit(3);
  }





}

void type_shift_exp(struct tree *parseT){
  token *check;

  if(parseT->kids[0]->prodrule == shift_expression){
    type_shift_exp(parseT->kids[0]);

    if(parseT->kids[2]->prodrule == postfix_expression){
      shift_post_helper(parseT->kids[2]);

    }
    else{
      check = parseT->kids[2]->leaf;
      shift_helper(check);
    }
  }
  else{
    if(parseT->kids[2]->prodrule == postfix_expression){
      shift_post_helper(parseT->kids[2]);
    }
    else{
      check = parseT->kids[2]->leaf;
      shift_helper(check);
    }

    struct type120* first_v = ht_get(currScope, parseT->kids[0]->leaf->text);
    if(first_v->base_type != CLASS_T){
      fprintf(stderr, "Shift expression stream variable error\n" );
      exit(3);
    }
    if(strcmp(first_v->u.class.type, "stream") != 0){
      fprintf(stderr, "Shift expression stream variable class error\n" );
      exit(3);
    }
  }


}

void shift_helper(token *toke){
  int product_r;
  struct type120* iden;
  product_r = toke->category;

  switch(product_r){
    case STRING:
      return;
    case ICON:
      return;
    case FCON:
      return;
    case CCON:
      return;
    case IDENTIFIER:
      iden = ht_get(currScope, toke->text);

      if(iden->base_type == INT_T || iden->base_type == DOUBLE_T || iden->base_type == CHAR_T){
        return;
      }
      else{
        fprintf(stderr, "Shift expression identifier error\n" );
        exit(3);
      }
    default:
    fprintf(stderr, "Shift expression error\n" );
    exit(3);

  }


}

void type_postfix_exp_2(struct tree *parseT){

  struct type120 *class_v, *member_v, *left, *right;
  struct hashtable_s *tempScope;
  int arrFlag = 0;


  left = ht_get(currScope, parseT->kids[0]->leaf->text);
  parseT = parseT->kids[2]->kids[0];

  if(parseT->kids[0]->prodrule == postfix_expression){
    arrFlag = 1;
  }

  if(arrFlag == 1){
    class_v = ht_get(currScope, parseT->kids[0]->kids[0]->leaf->text);
  }
  else{
    class_v = ht_get(currScope, parseT->kids[0]->leaf->text);
  }

  if(arrFlag == 1){
    tempScope = class_v->u.array.elemtype->u.class.private;
  }
  else{
    tempScope = class_v->u.class.private;
  }


  member_v = ht_get(tempScope, parseT->kids[2]->leaf->text);

  right = member_v->u.function.elemtype;
  type_compare(-1, left, right);


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
      if(iter->kids[0]->prodrule == postfix_expression){
        mult_postfix_helper(iter->kids[0], left, iter->kids[1]->prodrule);
      }
      else{
        type_compare(iter->kids[1]->prodrule, left, ht_get(currScope, iter->kids[0]->leaf->text));
      }
    }

    if(iter->kids[2]->prodrule == postfix_expression){
        mult_postfix_helper(iter->kids[2], left, iter->kids[1]->prodrule);
    }
    else if(iter->kids[2]->prodrule > 0){
      type_compare(iter->kids[1]->prodrule, left, ht_get(currScope, iter->kids[2]->leaf->text));
    }
    else{
      mult_helper(parseT->kids[2], left);
    }

  }

}

void mult_postfix_helper(struct tree *parseT, struct type120 *left, int oper){
  struct type120 *right;

  right = ht_get(currScope, parseT->kids[0]->leaf->text);

  type_compare(oper, left, right);
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
        break;
      case PLUS:
        if(leftT == INT_T || leftT == DOUBLE_T){

        }
        else{
          fprintf(stderr, "Type Error: Operator/Operand error\n");
          fprintf(stderr, "Operator: %d Operand: %d\n", leftT, operand);
          exit(3);
        }
        break;
      case MUL:
        if(leftT == INT_T || leftT == DOUBLE_T){

        }
        else{
          fprintf(stderr, "Type Error: Operator/Operand error\n");
          fprintf(stderr, "Operator: %d Operand: %d\n", leftT, operand);
          exit(3);
        }
        break;
      case DIV:
        if(leftT == INT_T || leftT == DOUBLE_T){

        }
        else{
          fprintf(stderr, "Type Error: Operator/Operand error\n");
          fprintf(stderr, "Operator: %d Operand: %d\n", leftT, operand);
          exit(3);
        }
        break;
      case MOD:
        if(leftT == INT_T || leftT == DOUBLE_T){

        }
        else{
          fprintf(stderr, "Type Error: Operator/Operand error\n");
          fprintf(stderr, "Operator: %d Operand: %d\n", leftT, operand);
          exit(3);
        }
        break;
      case LT:
        if(leftT == INT_T || leftT == DOUBLE_T){

        }
        else{
          fprintf(stderr, "Type Error: Operator/Operand error\n");
          fprintf(stderr, "Operator: %d Operand: %d\n", leftT, operand);
          exit(3);
        }
        break;
      case GT:
        if(leftT == INT_T || leftT == DOUBLE_T){

        }
        else{
          fprintf(stderr, "Type Error: Operator/Operand error\n");
          fprintf(stderr, "Operator: %d Operand: %d\n", leftT, operand);
          exit(3);
        }
        break;
      default:
        fprintf(stderr, "I don't recognize: %d\n", operand);
    }

}

void scope_change(struct tree *parseT){
  struct hashtable_s* newScope;
  int productRule;

  productRule = parseT->kids[1]->prodrule;


  switch(productRule){
    case declarator:
      newScope = ht_get(currScope, parseT->kids[1]->kids[1]->kids[0]->leaf->text)->u.function.sources;
      break;
    case direct_declarator:
      newScope = ht_get(currScope, parseT->kids[1]->kids[0]->leaf->text)->u.function.sources;
      break;
    case direct_declarator-1:
      newScope = scope_c_func_help(parseT->kids[1]);
      break;
    default:
      fprintf(stderr, "error in scope change\n");
      exit(3);
  }

  currScope = newScope;

}

struct hashtable_s* scope_c_func_help(struct tree *parseT){
  struct hashtable_s* retScope;
  struct type120* class_v;
  struct type120* func_v;

  class_v = ht_get(global, parseT->kids[0]->leaf->text);

  func_v = ht_get(class_v->u.class.private, parseT->kids[2]->leaf->text);


  retScope = func_v->u.function.sources;

  return retScope;
}

void init_type(){
  currScope = global;

  if(currScope == NULL){
    fprintf(stderr, "%s\n", "Type.c: init_type: Error with global scope reporting null");
    exit(3);
  }

}
