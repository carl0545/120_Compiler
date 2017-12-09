/* INTERMEDIATE.C */
#include "intermediate.h"
#include "nonterms.h"
#include "120gram.h"

void codeGen(struct tree *parseT){
  int i;
  if (parseT == NULL){
    return;
  }

  for(i = 0; i < parseT->nkids; i++){
    codeGen(parseT->kids[i]);
  }

  switch (parseT->prodrule) {

    case assignment_expression:
      parseT->place = parseT->kids[0]->place;
      parseT->code = concat(parseT->kids[2]->code, gen(O_ASN, parseT->kids[0]->place, parseT->kids[2]->place, NULL));
      break;

    case jump_statement:
      parseT->code = concat(parseT->kids[1]->code, gen(O_RET, parseT->kids[1]->place, NULL, NULL));
      break;

    case function_definition-1:
      parseT->code = precat(parseT->kids[3]->code, proc_gen(D_PROC, parseT->kids[1]->kids[0]->leaf->text));
      break;


      /*
    case IDENTIFIER:
      parseT->code = NULL;
      break;
      */

    default:
      /* default is: concatenate our children's code */
      parseT->code = NULL;
      printf("hmmm: %d\n", parseT->prodrule);
      for(i=0; i < parseT->nkids; i++){
         if(parseT->kids[i] == NULL){
            continue;
         }
         parseT->code = concat(parseT->code, parseT->kids[i]->code);
      }

  }


}

void print_main(struct instr *head, FILE *fpi){

  fprintf(fpi, ".global\n");

  global_print_helper(global, fpi);


  fprintf(fpi, ".code\n");

  print_instr(head, fpi);


}

void global_print_helper(struct hashtable_s *scopeG, FILE *fpi){
  if(scopeG == NULL){
    return;
  }

  int size_h;
  size_h = scopeG->size;


  for(int k = 0; k < size_h; k++){
    struct type120 *type_v;
    struct entry_s *entry_v;

    if(scopeG->table[k] == NULL){
      continue;
    }



    type_v = scopeG->table[k]->value;
    entry_v = scopeG->table[k];

    if(type_v->isConst){
      continue;
    }

    if(type_v->base_type == FUNCTION_T){
      continue;
    }

    int size_o;

    if(type_v->base_type == CHAR_T){
      size_o = 1;
    }
    else{
      size_o = 8;
    }

    fprintf(fpi, "  %s/%d/%d\n", entry_v->key, type_v->place.offset, size_o);




  }


}

void print_instr(struct instr *head, FILE *fpi){
  if(head == NULL)
    return;

  int op_i;
  struct addr *dest_i, *src1_i, *src2_i;


  op_i = head->opcode;

  if(op_i < D_GLOB){

    dest_i = head->dest;
    src1_i = head->src1;
    src2_i = head->src2;
  }




  switch (op_i) {
    case O_ASN:
      fprintf(fpi, "  ");
      fprintf(fpi, "O_ASN ");
      print_region_helper(fpi, dest_i->region, dest_i->offset);
      print_region_helper(fpi, src1_i->region, src1_i->offset);
      fprintf(fpi, "\n");
      break;
    case O_RET:
      fprintf(fpi, "  ");
      fprintf(fpi, "O_RET ");
      print_region_helper(fpi, dest_i->region, dest_i->offset);
      fprintf(fpi, "\n");
      break;
    case D_PROC:
      fprintf(fpi, "%s:", head->proc_name);
      fprintf(fpi, "\n");
      break;

  }

  print_instr(head->next, fpi);


}

void print_region_helper(FILE *fpi, int reg, int off){

  switch(reg){
    case R_GLOBAL:
      fprintf(fpi, " global:%d, ", off);
      break;
    case R_LOCAL:
      fprintf(fpi, " local:%d, ", off);
      break;
    case R_CLASS:
      fprintf(fpi, " class:%d, ", off);
      break;
    case R_LABEL:
      fprintf(fpi, " label:%d, ", off);
      break;
    case R_CONST:
      fprintf(fpi, " const:%d, ", off);
      break;
    case R_PARAM:
      fprintf(fpi, " param:%d, ", off);
      break;
    case R_CONSTC:
      fprintf(fpi, " chconst:%d, ", off);
      break;
    case R_FLOAT:
      fprintf(fpi, " fconst:%d, ", off);
      break;
    default:
      fprintf(stderr, "ERROR IN print_region_helper\n");
      exit(5);

  }


}

void init_imd_globals(){
  label_counter = 0;
  global_r_counter = 0;
  local_r_counter = 0;
  param_r_counter = 0;
  float_r_counter = 0;

  i_scope = global;

  if(i_scope == NULL){
    fprintf(stderr, "ERROR in init_imd_globals, i_scope is null\n");
  }

}

struct addr* label_create(){
  struct addr *lab = addr_create(R_LOCAL, label_counter);
  label_counter++;
  return lab;
}

struct addr* addr_create(int reg, int offs){
  struct addr* newAddr;
  newAddr = malloc(sizeof(struct addr));

  newAddr->region = reg;
  newAddr->offset = offs;

  return newAddr;
}

void gen_place(struct tree *parseT){
  int i;

  if(parseT == NULL){
    return;
  }

  switch(parseT->prodrule){
    case IDENTIFIER:
      place_helper(parseT);
      break;
    case function_definition-1:{
      struct type120 *scopeH;

      scopeH = ht_get(i_scope, parseT->kids[1]->kids[0]->leaf->text);
      i_scope = scopeH->u.function.sources;
      }
      break;
    case ICON:{
      struct addr *newA;

      newA = addr_create(R_CONST, parseT->leaf->ival);
      parseT->place = newA;
      parseT->code = NULL;
      }
      break;
    case CCON:{
      struct addr *newA;
      int charac = (int) parseT->leaf->text[0];

      newA = addr_create(R_CONSTC, charac);
      parseT->place = newA;
      parseT->code = NULL;
      }
      break;
    case FCON:{
      struct addr *newA;

      newA = addr_create(R_FLOAT, float_r_counter);
      float_r_counter += 8;
      parseT->place = newA;
      parseT->code = NULL;
      }
      break;
    case STRING:
      break;


  }


  for(i = 0; i < parseT->nkids; i++){
     gen_place(parseT->kids[i]);
  }




}


void gen_temp(struct tree *parseT){
  int i;

  if(parseT == NULL){
    return;
  }

  switch(parseT->prodrule){
    case additive_expression:{
      struct addr *temp_v;
      temp_v = temp_create();

      parseT->place = temp_v;

      break;
    }
    case multiplicative_expression:{
      struct addr *temp_v;
      temp_v = temp_create();

      parseT->place = temp_v;

      break;
    }

  }



  for(i = 0; i < parseT->nkids; i++){
     gen_temp(parseT->kids[i]);
  }




}

void place_helper(struct tree *parseT){
  struct addr *newPlace, *oldPlace;
  struct type120 *ident;
  int nReg, nOff;

  ident = ht_get(i_scope, parseT->leaf->text);

  if(ident == NULL){
    return;
  }

  printf("OKAY ONE SEC: %s\n",parseT->leaf->text);

  if(ident->base_type == FUNCTION_T)
    return;


  oldPlace = addr_create(ident->place.region, ident->place.offset);

  nOff = oldPlace->offset;

  printf("OKAY WAIT A MINTE: %d\n", oldPlace->region);
  switch(oldPlace->region){
    case 0: //GLOBAL_H:
      nReg = R_GLOBAL;
      break;
    case 1: //LOCAL_H:
      nReg = R_LOCAL;
      break;
    case 2: //PARAM_H:
      nReg = R_PARAM;
      break;
  }

  newPlace = addr_create(nReg, nOff);

  parseT->place = newPlace;

}

void gen_follows(struct tree *parseT){
  int i;

  if(parseT == NULL){
     return;
  }

   switch (parseT->prodrule) {
    case statement_seq-1: /* statement_seq : statement_seq statement */
      parseT->kids[0]->follow = parseT->kids[1]->first;
	    parseT->kids[1]->follow = parseT->follow;
   	  break;
    case compound_statement: /* compstmt : '{' statement_seq_opt '}' */
      if (parseT->kids[1] != NULL)
        parseT->kids[1]->follow = parseT->follow;
   	  break;
    case function_definition-1: /* funcdef : declarator ctor_init_opt body */
      parseT->kids[3]->follow = label_create();
      /* .code must have this label and a return at the end! */
   	  break;
   /* ... other cases? ... */
    default:
      break;
   }



   for(i = 0; i < parseT->nkids; i++){
      gen_follows(parseT->kids[i]);
   }


}

struct addr* temp_create(){
  struct addr *newTemp = addr_create(R_LOCAL, local_r_counter);
  local_r_counter += 8;
  return newTemp;
}

void gen_first(struct tree *parseT){
  if(parseT == NULL){
    return;
  }
  int i;

  for(i=0; i < parseT->nkids; i++){
    gen_first(parseT->kids[i]);
  }

  switch (parseT->prodrule) {
     case labeled_statement:
        parseT->first = parseT->kids[2]->first;
        break;
     case labeled_statement-1:
        parseT->first = parseT->kids[3]->first;
        break;
     case expression_statement:
        parseT->first = label_create();
        break;

     case compound_statement:
        parseT->first = label_create();
        break;

     case selection_statement:
        parseT->first = label_create();
        break;

     case iteration_statement:
        parseT->first = label_create();
        break;

     case jump_statement:
        parseT->first = label_create();
        break;

     case declaration_statement:
        parseT->first = label_create();
        break;
     default:
        break;
     }

}




int ht_set_size(struct hashtable_s *hashtable, int *local, int *global, int *param, enum regions_h currRegion){
  if(hashtable == NULL){
    return 0;
  }

  int size_h;
  size_h = hashtable->size;

  int totalsize = 0;

  for(int k = 0; k < size_h; k++){
    struct type120 *type_v;
    struct entry_s *entry_v;

    if(hashtable->table[k] == NULL){
      continue;
    }



    type_v = hashtable->table[k]->value;
    entry_v = hashtable->table[k];

    if(type_v->isConst){
      continue;
    }

    //printf("%s: %d\n", hashtable->table[k]->key ,type_v->base_type);

    switch(type_v->base_type){
        case INT_T:
          type_v->place.region = currRegion;
          type_v->place.offset = getOffset(currRegion, global, local, param, INT_SIZE, &totalsize);
        break;

        case DOUBLE_T:
          type_v->place.region = currRegion;
          type_v->place.offset = getOffset(currRegion, global, local, param, DOUBLE_SIZE, &totalsize);
        break;

        case CHAR_T:
        type_v->place.region = currRegion;
        type_v->place.offset = getOffset(currRegion, global, local, param, CHAR_SIZE, &totalsize);
        break;

        case BOOL_T:
        type_v->place.region = currRegion;
        type_v->place.offset = getOffset(currRegion, global, local, param, INT_SIZE, &totalsize);
        break;

        case ARRAY_T: {
        int size_a = type_v->u.array.size;
        int increment_a = getArrayType(type_v->u.array.elemtype->base_type);
        type_v->place.region = currRegion;
        type_v->place.offset = getOffset(currRegion, global, local, param, increment_a, &totalsize);
        arrayOffset(currRegion, global, local, param, increment_a, size_a, &totalsize);
        break;
        }

        break;
        case FUNCTION_T:

          ht_set_size(type_v->u.function.sources, local, global, param, LOCAL_H);
          setParam(type_v->u.function.parameters, global, local, param, &totalsize, PARAM_H);

        break;
        case CLASS_T:{
          int size_c;
          size_c = ht_set_size(type_v->u.class.private, local, global, param, LOCAL_H);

          printf("going into private:\n\n");
          type_v->place.region = currRegion;
          type_v->place.offset = getOffset(currRegion, global, local, param, size_c, &totalsize);
          printf("coming out of private\n\n");
        }
        break;

        default:
          fprintf(stderr, "Error  in switch statement of ht_set_size, base_type: %d\n", type_v->base_type);
          exit(3);

    }





    //Iterate through list as well
    while(entry_v->next != NULL){
      struct type120 *type_w;
      entry_v = entry_v->next;

      type_w = entry_v->value;
      printf("inside: %s: %d\n", entry_v->key, type_w->base_type);
      type_v = type_w;
      switch(type_v->base_type){
          case INT_T:
            type_v->place.region = currRegion;
            type_v->place.offset = getOffset(currRegion, global, local, param, INT_SIZE, &totalsize);
          break;

          case DOUBLE_T:
            type_v->place.region = currRegion;
            type_v->place.offset = getOffset(currRegion, global, local, param, DOUBLE_SIZE, &totalsize);
          break;

          case CHAR_T:
          type_v->place.region = currRegion;
          type_v->place.offset = getOffset(currRegion, global, local, param, CHAR_SIZE, &totalsize);
          break;

          case BOOL_T:
          type_v->place.region = currRegion;
          type_v->place.offset = getOffset(currRegion, global, local, param, INT_SIZE, &totalsize);
          break;

          case ARRAY_T: {
          int size_a = type_v->u.array.size;
          int increment_a = getArrayType(type_v->u.array.elemtype->base_type);
          type_v->place.region = currRegion;
          type_v->place.offset = getOffset(currRegion, global, local, param, increment_a, &totalsize);
          arrayOffset(currRegion, global, local, param, increment_a, size_a, &totalsize);
          break;
          }

          break;
          case FUNCTION_T:
            ht_set_size(type_v->u.function.sources, local, global, param, LOCAL_H);
            //create a function to deal with parameters of a function
            setParam(type_v->u.function.parameters, global, local, param, &totalsize, PARAM_H);
          break;
          case CLASS_T:{
            int size_c;
            size_c = ht_set_size(type_v->u.class.private, local, global, param, LOCAL_H);

            printf("going into private:\n\n");
            type_v->place.region = currRegion;
            type_v->place.offset = getOffset(currRegion, global, local, param, size_c, &totalsize);
            printf("coming out of private\n\n");
          }
          break;

          default:
            fprintf(stderr, "Error  in switch statement of ht_set_size, base_type: %d\n", type_v->base_type);
            exit(3);

      }

    }



  }

  return totalsize;
}

void setParam(struct listnode *head, int *global, int *local, int *param, int *total, enum regions_h currRegion){

  for(int k = 1; k < listSize(head)+1; k++){
    struct type120 *param_v;

    param_v = listGet(head, k);

    switch(param_v->base_type){
        case INT_T:
          param_v->place.region = currRegion;
          param_v->place.offset = getOffset(currRegion, global, local, param, INT_SIZE, total);
        break;

        case DOUBLE_T:
          param_v->place.region = currRegion;
          param_v->place.offset = getOffset(currRegion, global, local, param, DOUBLE_SIZE, total);
        break;

        case CHAR_T:
        param_v->place.region = currRegion;
        param_v->place.offset = getOffset(currRegion, global, local, param, CHAR_SIZE, total);
        break;

        case BOOL_T:
        param_v->place.region = currRegion;
        param_v->place.offset = getOffset(currRegion, global, local, param, INT_SIZE, total);
        break;

        case ARRAY_T: {
        int size_a = param_v->u.array.size;
        int increment_a = getArrayType(param_v->u.array.elemtype->base_type);
        param_v->place.region = currRegion;
        param_v->place.offset = getOffset(currRegion, global, local, param, increment_a, total);
        arrayOffset(currRegion, global, local, param, increment_a, size_a, total);
        break;
        }

        break;
        case FUNCTION_T:

          ht_set_size(param_v->u.function.sources, local, global, param, LOCAL_H);
          setParam(param_v->u.function.parameters, global, local, param, total, PARAM_H);

        break;
        case CLASS_T:{
          int size_c;
          size_c = ht_set_size(param_v->u.class.private, local, global, param, LOCAL_H);

          printf("going into private:\n\n");
          param_v->place.region = currRegion;
          param_v->place.offset = getOffset(currRegion, global, local, param, size_c, total);
          printf("coming out of private\n\n");
        }
        break;

        default:
          fprintf(stderr, "Error  in switch statement of ht_set_size, base_type: %d\n", param_v->base_type);
          exit(3);

    }


  }


}



void arrayOffset(enum regions_h currRegion, int *global, int *local, int *param, int increment, int size_a, int *total){
  size_a = size_a - 1;

  if(size_a <= 0){
    return;
  }

  switch(currRegion){
    case GLOBAL_H:
      *total += increment * size_a;
      *global += increment * size_a;
      break;
    case LOCAL_H:
      *total += increment * size_a;
      *local += increment * size_a;
      break;
    case PARAM_H:
      *total += increment * size_a;
      *param += increment * size_a;
      break;

    default:
      fprintf(stderr, "Error in arrayOffset\n");
      exit(3);
  }

}

int getArrayType(int baseT){
  switch(baseT){

    case 0: //INT_T
      return INT_SIZE;
      break;
    case 1: //DOUBLE_T
      return DOUBLE_SIZE;
      break;
    case 2: //CHAR_T
      return CHAR_SIZE;
      break;
    case 3: //BOOL_T
      return INT_SIZE;
      break;
    case 6: //CLASS_T
      fprintf(stderr, "CLASS ARRAY SIZE not implemented in getArraytype\n");
      exit(3);
      break;
    default:
      fprintf(stderr, "ERROR IN getArrayType\n");
      exit(3);

  }

return -1;
}

int getOffset(enum regions_h currRegion, int *global, int *local, int *param, int increment, int *total){
  int retVal;
  switch(currRegion){
    case GLOBAL_H:
      *total += increment;
      retVal = *global;
      *global += increment;
      break;
    case LOCAL_H:
      *total += increment;
      retVal = *local;
      *local += increment;
      break;
    case PARAM_H:
      *total += increment;
      retVal = *param;
      *param += increment;
      break;

    default:
      fprintf(stderr, "Error in getOffset\n");
      exit(3);
  }

return retVal;
}

void printSize(struct hashtable_s *hashtable){
  if(hashtable == NULL){
    return;
  }

  int size_h;
  size_h = hashtable->size;

  for(int k = 0; k < size_h; k++){
    struct type120 *type_v;
    struct entry_s *entry_v;

    if(hashtable->table[k] == NULL){
      continue;
    }

    type_v = hashtable->table[k]->value;
    entry_v = hashtable->table[k];

    if(type_v->isConst){
      continue;
    }


    if(type_v->base_type == CLASS_T){
      printSize(type_v->u.class.private);
      printf("CLASS Variable: %s has region %d with offset %d\n", entry_v->key, type_v->place.region, type_v->place.offset);
    }
    else if(type_v->base_type == FUNCTION_T){
      printSize(type_v->u.function.sources);
      printParam(type_v->u.function.parameters, entry_v);
    }
    else{
      printf("Variable: %s has region %d with offset %d\n", entry_v->key, type_v->place.region, type_v->place.offset);
    }


  }



}

void printParam(struct listnode *head, struct entry_s *entry_v){
  if(head == NULL){
    return;
  }

  struct listnode *traverse;
  traverse = head;
  int count = 1;

  traverse = traverse->next; //error probs
  while(traverse != NULL){
    printf("function: %s param: %d has region %d with offset %d\n", entry_v->key, count, traverse->type->place.region, traverse->type->place.offset);
    count++;
    traverse = traverse->next;
  }

}
