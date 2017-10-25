#include "symtable.h"
#include "nonterms.h"
#include "120gram.h"
#include <string.h>

void populateSymbolTable(struct tree *parseT){
  int i;

  if(parseT == NULL){

    return;
  }

  switch(parseT->prodrule){
    case class_specifier:
      //printf("class not implemented yet\n");
      //not used
      break;
    case function_definition:
      //printf("functions not implemented yet1\n");
      //not used
      return;
      break;
    case expression_statement:
      handle_expr_state(parseT);
      return;
    case function_definition-1:
      handle_func_def(parseT);
      return;
    case simple_declaration:{
        if(parseT->kids[0]->prodrule == class_specifier){

          handle_class_spec(parseT->kids[0]);
          return;
        }
        if(parseT->kids[1]->prodrule == init_declarator_list){
          handle_init_list(parseT);

          return;
        }
        handle_init_decl(parseT);
      }

  }//end of main switch

    for(i = 0; i < parseT->nkids; i++){
      populateSymbolTable(parseT->kids[i]);
    }

}

/*
* Checks to see if variable/function has been declared in an expression
*
*/
void handle_expr_state(struct tree *parseT){
    if(ht_get(curr, parseT->kids[0]->kids[0]->leaf->text) == NULL){
      fprintf(stderr, "SEMANTIC ERROR: use of an undeclared var\n");
      exit(3);
    }
}

void handle_init_list(struct tree *parseT){
  int base = find_base_type(parseT->kids[0]->leaf->category);

  struct tree *iter = parseT->kids[1];
  struct tree *prev = iter;
  int count = 1;
  while(iter->prodrule == init_declarator_list){
    count++;
    iter = iter->kids[0];
  }

  int index = count - 1;

  for(int i = 0; i < count-1; i++){
    iter = parseT->kids[1];
    struct tree *prev = iter;
    struct type120 *newV = malloc(sizeof(struct type120));
    while(index > i){
      prev = iter;
      iter = iter->kids[0];
      index--;
    }
    index = count - 1;

    if(i == 0){
      newV->base_type = base;
      if(iter->kids[1] != NULL){
        newV->pointer = true;
      }
      else{
        newV->pointer = false;
      }
      //printf("CURRENT TEXT: %s\n", iter->kids[0]->leaf->text);
      ht_set(curr, iter->kids[0]->leaf->text, newV);
      //printf("CURRENT TEXT: %s\n", prev->kids[2]->kids[0]->leaf->text);
      ht_set(curr, prev->kids[2]->kids[0]->leaf->text, newV);
    }
    else{
      newV->base_type = base;
      if(iter->kids[2]->kids[1] != NULL){
        newV->pointer = true;
      }
      else{
        newV->pointer = false;
      }

      //printf("CURRENT TEXT: %s\n", prev->kids[2]->kids[0]->leaf->text);
      ht_set(curr, prev->kids[2]->kids[0]->leaf->text, newV);
    }

    //printf("YOYOYO\n");

  }


}

void handle_class_spec(struct tree *parseT){
  struct type120* newType;
  char *nameKey = parseT->kids[0]->kids[1]->leaf->text;
  newType = malloc(sizeof(struct type120));
  newType->base_type = CLASS_T;

  if(parseT->kids[2]->prodrule == member_specification){
    //printf("IM IN member_specification 1\n");
    handle_member_spec1(parseT->kids[2], newType);
  }
  else if(parseT->kids[2]->prodrule == member_specification-1){
    //printf("IM IN member_specification 2\n");
    handle_member_spec2(parseT->kids[2], newType);
  }

  ht_set(curr, parseT->kids[0]->kids[1]->leaf->text, newType);

}
void handle_c_func_decl(struct tree *parseT){
  struct type120 *funcVar = malloc(sizeof(struct type120));
  funcVar->base_type = FUNCTION_T;

  funcVar->u.function.elemtype = malloc(sizeof(struct type120));
  funcVar->u.function.elemtype->base_type = find_base_type(parseT->kids[0]->kids[0]->leaf->category);

  //parameters part

  //no params
  if(parseT->kids[0]->kids[1]->kids[2] == NULL){
    ht_set(curr, parseT->kids[0]->kids[1]->kids[0]->leaf->text, funcVar);
  }
  else if(parseT->kids[0]->kids[1]->kids[2]->prodrule == parameter_declaration){
    //one param
    struct type120 *param = malloc(sizeof(struct type120));
    funcVar->u.function.parameters = malloc(sizeof(struct listnode));

    if(parseT->kids[0]->kids[1]->kids[2]->kids[1]->prodrule == abstract_declarator){
      param->pointer = true;
    }
    else{
      param->pointer = false;
    }

    param->base_type = find_base_type(parseT->kids[0]->kids[1]->kids[2]->kids[0]->leaf->category);

    add(&funcVar->u.function.parameters, param);

    ht_set(curr, parseT->kids[0]->kids[1]->kids[0]->leaf->text, funcVar);

  }
  else if(parseT->kids[0]->kids[1]->kids[2]->prodrule == parameter_declaration_list){
    funcVar->u.function.parameters = malloc(sizeof(struct listnode));

    struct tree *iter = parseT->kids[0]->kids[1]->kids[2];
    int count = 1;
    while(iter->prodrule == parameter_declaration_list){
      count++;
      iter = iter->kids[0];
    }
    int index = count - 1;

    for(int i = 0; i < count; i++){
      iter = parseT->kids[0]->kids[1]->kids[2];
      struct type120 *param = malloc(sizeof(struct type120));
      while(index > i){
        iter = iter->kids[0];
        index--;
      }
      index = count - 1;

      if(i == 0){
        param->base_type = find_base_type(iter->kids[0]->leaf->category);
        if(iter->kids[1] != NULL){
          param->pointer = true;
        }
        else{
          param->pointer = false;
        }
      }
      else{
        param->base_type = find_base_type(iter->kids[2]->kids[0]->leaf->category);
        if(iter->kids[2]->kids[1] != NULL){
          param->pointer = true;
        }
        else{
          param->pointer = false;
        }
      }

      add(&funcVar->u.function.parameters, param);

    }

    ht_set(curr, parseT->kids[0]->kids[1]->kids[0]->leaf->text, funcVar);
    //exit(3);

  }
}

void handle_member_spec1(struct tree *parseT, struct type120 *newType){

  if(parseT->kids[0]->kids[1]->prodrule == direct_declarator){
    //printf("CLASS direct_declarator\n");
    handle_c_func_decl(parseT);
    //exit(3);
  }
  else if(parseT->kids[0]->kids[1]->prodrule == IDENTIFIER){
    //printf("CLASS variable dec\n");
    struct type120 *classVar = malloc(sizeof(struct type120));
    classVar->base_type = find_base_type(parseT->kids[0]->kids[0]->leaf->category);
    ht_set(curr, parseT->kids[0]->kids[1]->leaf->text, classVar);
  }
  else{
    fprintf(stderr, "ERROR IN handle_member_spec1\n");
    exit(3);
  }

  if(parseT->kids[1] == NULL){
    printf("oh alright\n");
    return;
  }

  if(parseT->kids[1]->prodrule == member_specification){
    //printf("IM IN member_specification 1\n");
    handle_member_spec1(parseT->kids[1], newType);
  }
  else if(parseT->kids[1]->prodrule == member_specification-1){
    //printf("IM IN member_specification 2\n");
    handle_member_spec2(parseT->kids[1], newType);
  }



}

void handle_member_spec2(struct tree *parseT, struct type120 *newType){
struct hashtable_s *classScope = ht_create(CLASSSIZE, curr);
struct hashtable_s *oldScope = curr;

  if(strcmp(parseT->kids[0]->leaf->text, "private") == 0 || strcmp(parseT->kids[0]->leaf->text, "protected") == 0){
    newType->u.class.private = classScope;
    curr = classScope;

  }
  else if(strcmp(parseT->kids[0]->leaf->text, "public") == 0){
    newType->u.class.private = classScope;
    curr = classScope;
  }

  if(parseT->kids[2] == NULL){
    //printf("oh alright1\n");
    return;
  }

  if(parseT->kids[2]->prodrule == member_specification){
    //printf("IM IN member_specification 1\n");
    handle_member_spec1(parseT->kids[2], newType);
  }
  else if(parseT->kids[2]->prodrule == member_specification-1){
    //printf("IM IN member_specification 2\n");
    handle_member_spec2(parseT->kids[2], newType);
  }



  curr = oldScope;

  //exit(1);
}

/*
*Handle function defintions
*/
void handle_func_def(struct tree *parseT){
  //check if main has already been declared if not see if this function is main
  bool pointerCheck;
  struct tree *checker;

  if(parseT->kids[1]->prodrule == declarator && parseT->kids[1]->kids[0]->prodrule == MUL){
    //printf("FUNCTION definition return does have pointer\n");
    pointerCheck = true;
  }
  else{
    //printf("FUNCTION definition return doesn't have pointer\n");
    pointerCheck = false;
  }



  if(mainCheck == false){
    if(checkMain(parseT)){
      mainCheck = true;
      struct type120 *mainT;
      mainT = malloc(sizeof(struct type120));
      mainT->base_type = FUNCTION_T;
      mainT->pointer = false;

      ht_set(curr, "main", mainT);
    }
  }

  if(mainCheck == true){
    //check if has been declared
    if(pointerCheck == false){
      if(ht_get(curr, parseT->kids[1]->kids[0]->leaf->text) == NULL && strcmp(parseT->kids[1]->kids[0]->leaf->text, "main") != 0){
        fprintf(stderr, "SEMANTIC ERROR: Function has not been declared before defintion\n");
        exit(3);
      }
    }
    else{
      if(ht_get(curr, parseT->kids[1]->kids[1]->kids[0]->leaf->text) == NULL && strcmp(parseT->kids[1]->kids[0]->leaf->text, "main") != 0){
        fprintf(stderr, "SEMANTIC ERROR: Function has not been declared before defintion\n");
        exit(3);
      }
    }
  }

  //Declares the function if before main
  if(mainCheck == false){

    struct type120 *newType;
    newType = malloc(sizeof(struct type120));
    newType->base_type = FUNCTION_T;

    //check if pointer
    if(parseT->kids[1]->prodrule == declarator && parseT->kids[1]->kids[0]->prodrule == MUL){
      //printf("FUNCTION definition return does have pointer\n");
      handle_func_decl(parseT, newType, true, true, true);
      checker = parseT->kids[1]->kids[1];
    }
    else{
      //printf("FUNCTION definition return doesn't have pointer\n");
      handle_func_decl(parseT, newType, false, true, true);
      checker = parseT->kids[1];
    }

  }

  else{
    if(parseT->kids[1]->prodrule == declarator && parseT->kids[1]->kids[0]->prodrule == MUL){
      checker = parseT->kids[1]->kids[1];
    }
    else{
      checker = parseT->kids[1];
    }

  }

  //Check if defintion return type and parameters match
  struct type120 *checkDec, *checkDefin;
  checkDefin = malloc(sizeof(struct type120));
  checkDefin->base_type = FUNCTION_T;
  if(parseT->kids[1]->prodrule == declarator && parseT->kids[1]->kids[0]->prodrule == MUL){
    checkDefin->pointer = true;
  }
  else{
    checkDefin->pointer = false;
  }
  handle_func_decl(parseT, checkDefin, checkDefin->pointer, true, false);


  checkDec = ht_get(curr, checker->kids[0]->leaf->text);

  if(checkDec->pointer == checkDefin->pointer){
    if(checkDec->base_type == checkDefin->base_type){
      //we made it
      if(checkParams(&checkDec->u.function.parameters, &checkDefin->u.function.parameters)){

      }
      else{
        fprintf(stderr, "SEMANTIC ERROR: Function declaration and definition params have different counts\n");
        exit(3);
      }
    }
    else{
      fprintf(stderr, "SEMANTIC ERROR: Function declaration and definition params have different counts\n");
      exit(3);
    }
  }
  else{
    fprintf(stderr, "SEMANTIC ERROR: Function declaration and definition params have different counts\n");
    exit(3);
  }


  //Enter the function scope, change scope of curr and recursively call populateSymbolTable
  //make sure it exits with changing curr back to global

  struct hashtable_s *newFuncScope = ht_create(FUNCTIONSIZE, curr);
  struct hashtable_s *oldScope;
  struct tree *newFuncTree;
  struct type120 *funct;

  funct = ht_get(curr, checker->kids[0]->leaf->text);
  //printf("inside: %s\n", checker->kids[0]->leaf->text);

  oldScope = curr;


  if(parseT->kids[3]->prodrule == compound_statement){
    //printf("I'm in compund statement\n");
    newFuncTree = parseT->kids[2];
  }
  else{
    //printf("I'm NOT in compund statement\n");
    exit(3);
  }
  funct->u.function.sources = ht_create(FUNCTIONSIZE, curr);
  //funct->u.function.sources = newFuncScope;

  curr = funct->u.function.sources;

  int k;
  for(k = 0; k < parseT->nkids; k++){
    populateSymbolTable(parseT->kids[k]);
  }
  //populateSymbolTable(newFuncTree);
  //funct->u.function.sources = curr;

  curr = global;
}
/*
*Checks if parameters are the same
*/
bool checkParams(struct listnode **p1, struct listnode **p2){
  int count1, count2;
  struct listnode *counter1, *counter2;
  count1 = 0;
  count2 = 0;

  counter1 = *p1;
  counter2 = *p2;


  while(counter1 != NULL){
    count1++;
    counter1 = counter1->next;

  }

  //printf("COUNT 1: %d\n", count1);

  while(counter2 != NULL){
    count2++;
    counter2 = counter2->next;
  }
  //printf("COUNT 2: %d\n", count2);

  if(count1 != count2){
    return false;
  }

  if(count1 == 0){
    return true;
  }

  counter1 = *p1;
  counter2 = *p2;

  counter1=counter1->next;
  counter2=counter2->next;

  int i;
  for(i = 0; i < count1-1; i++){
    if(counter1->type->base_type != counter2->type->base_type || counter1->type->pointer != counter2->type->pointer){
      fprintf(stderr, "%s\n", "SEMANTIC ERROR: function parameters do not match");
      exit(3);
    }


    counter1=counter1->next;
    counter2=counter2->next;
  }


  return true;
}

/*
*Check if the function is the main function
*/
bool checkMain(struct tree *parseT){
  if(strcmp(parseT->kids[1]->kids[0]->leaf->text, "main") == 0){
    return true;
  }
  else{
    return false;
  }

}
/*
*Handle initializing variables and functions
*/
void handle_init_decl(struct tree *parseT){
  struct type120 *newType;
  newType = malloc(sizeof(struct type120));
  //Variable Declaration
  if(parseT->kids[1]->kids[0]->leafCheck == 1){
    newType->base_type = find_base_type(parseT->kids[0]->leaf->category);
    //check if pointer;
    newType->pointer = false;
    ht_set(curr, parseT->kids[1]->kids[0]->leaf->text, newType);
    //printf("adding a variable\n");

    return;
  }
  //Function Declaration/Array Declaration
  else if(parseT->kids[1]->kids[0]->leafCheck == 0){
    //fprintf(stderr, "THIS IS A FUNCTION/ARRAY DECLARATOR\n");
    //printf("CHECK; %s\n", humanreadable(parseT->kids[1]->kids[0]->prodrule));
    //check for pointer
    if(parseT->kids[1]->kids[0]->kids[0]->prodrule == MUL && parseT->kids[1]->kids[0]->kids[1]->prodrule == direct_declarator){
      //is a pointer
      //printf("I'm in a pointer return type: %s\n", parseT->kids[1]->kids[0]->kids[0]->leaf->text);
      newType->pointer = true;
      //check for double pointer will need to fix later
      if(parseT->kids[1]->kids[0]->kids[1]->prodrule == declarator){
        fprintf(stderr, "%s\n", "Double pointers not supported\n" );
        exit(2);
      }
      newType->base_type = FUNCTION_T;
      handle_func_decl(parseT, newType, true, false, true);
      return;
    }
    switch(parseT->kids[1]->kids[0]->kids[1]->leaf->category){
      case(LB):
        //printf("INSIDE ARRAY\n");
        newType->base_type = ARRAY_T;
        newType->pointer = false;
        handle_arr_decl(parseT, newType);
        break;
      case(LP):
        //printf("INSIDE FUNCTION\n");
        newType->base_type = FUNCTION_T;
        newType->pointer = false;
        handle_func_decl(parseT, newType, false, false, true);
        break;
      case(IDENTIFIER): //means single pointer
        if(parseT->kids[1]->kids[0]->kids[0]->leaf->category == MUL){
          newType->base_type = find_base_type(parseT->kids[0]->leaf->category);
          newType->pointer = true;
          ht_set(curr, parseT->kids[1]->kids[0]->kids[1]->leaf->text, newType);
          break;
        }
      default:
        fprintf(stderr, "ERROR WITHIN handle_init_decl\n");
    }

  }
  else{
    fprintf(stderr, "ERROR: in handle_init_decl\n");
  }

}
/*
*Handle array declarations
*/
void handle_arr_decl(struct tree* parseT, struct type120* type){

  struct type120 *elem = malloc(sizeof(struct type120));

  //check if no size
  if(parseT->kids[1]->kids[0]->kids[2] == NULL){
    type->u.array.size = 0;

  }
  else{
    type->u.array.size = parseT->kids[1]->kids[0]->kids[2]->leaf->ival;

  }
  elem->base_type = find_base_type(parseT->kids[0]->leaf->category);
  type->u.array.elemtype = elem;

  ht_set(curr, parseT->kids[1]->kids[0]->kids[0]->leaf->text, type);

}

void handle_func_decl(struct tree *parseT, struct type120 *type, bool pointer, bool defin, bool set){
  struct tree *checker;

  if(defin == false){
    if(pointer == false){
      //printf("function is a non pointer\n");
      checker = parseT->kids[1]->kids[0];
    }
    else{
      //printf("function is a pointer\n");
      checker = parseT->kids[1]->kids[0]->kids[1];
    }
  }
  if(defin == true){
    if(pointer == false){
      checker = parseT->kids[1];
    }
    else{
      checker = parseT->kids[1]->kids[1];
    }
  }
  type->u.function.elemtype = malloc(sizeof(struct type120));
  //Sets the return type

  type->u.function.elemtype->base_type = find_base_type(parseT->kids[0]->leaf->category);


  //List parameters
  if(checker->kids[2] == NULL){
    //no parameters
    struct type120 *param = malloc(sizeof(struct type120));
    //type->u.function.parameters = malloc(sizeof(struct listnode));//might be an error later
    param->base_type = VOID_T;
    //add(&type->u.function.parameters, param);

    if(set == true){
      ht_set(curr, checker->kids[0]->leaf->text, type);
    }
    return;

  }
  else if(checker->kids[2]->prodrule == parameter_declaration){
    //one parameters
    struct type120 *param = malloc(sizeof(struct type120));
    type->u.function.parameters = malloc(sizeof(struct listnode));

    if(checker->kids[2]->kids[1] != NULL){
      //pointer param
      param->pointer = true;
      //printf("pointer\n");
    }
    else{
      param->pointer = false;
    }

    param->base_type = find_base_type(checker->kids[2]->kids[0]->leaf->category);

    add(&type->u.function.parameters, param);

    if(set == true){
      ht_set(curr, checker->kids[0]->leaf->text, type);
    }
    //ht_set(curr, checker->kids[0]->leaf->text, type);
    return;

  }
  else if(checker->kids[2]->prodrule == parameter_declaration_list){
    //multiple parameters
    type->u.function.parameters = malloc(sizeof(struct listnode));

    struct tree *iter = checker->kids[2];
    int count = 1;
    while(iter->prodrule == parameter_declaration_list){
      count++;
      iter = iter->kids[0];
    }
    int index = count - 1;

    for(int i = 0; i < count; i++){
      iter = checker->kids[2];
      struct type120 *param = malloc(sizeof(struct type120));
      while(index > i){
        iter = iter->kids[0];
        index--;
      }
      index = count - 1;

      if(i == 0){
        param->base_type = find_base_type(iter->kids[0]->leaf->category);
        if(iter->kids[1] != NULL){
          param->pointer = true;
        }
        else{
          param->pointer = false;
        }
      }
      else{
        param->base_type = find_base_type(iter->kids[2]->kids[0]->leaf->category);
        if(iter->kids[2]->kids[1] != NULL){
          param->pointer = true;
        }
        else{
          param->pointer = false;
        }
      }

      add(&type->u.function.parameters, param);

    }


    if(set == true){
      ht_set(curr, checker->kids[0]->leaf->text, type);
      //printf("\n\nTEST: %s", checker->kids[0]->leaf->text);
    }
    //ht_set(curr, parseT->kids[1]->kids[0]->kids[0]->leaf->text, type);
    return;

  }
  else{
    fprintf(stderr, "%s\n", "Semantic Error" );
    exit(3);
  }


}


void initGlobal(){

  global = ht_create(ROOTSIZE, NULL);
  curr = global;
  mainCheck = false;

}

int find_base_type(int token){
  switch(token){
    case(INT):
      return INT_T;
      break;
    case(DOUBLE):
      return DOUBLE_T;
      break;
    case(CHAR):
      return CHAR_T;
      break;
    case(BOOL):
      return BOOL_T;
      break;
    default:
    //check for defined other types
    fprintf(stderr, "%s\n", "Variable declaration has an unregonized type" );
    exit(3);
  }


}
