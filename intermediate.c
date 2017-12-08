/* INTERMEDIATE.C */
#include "intermediate.h"

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

    printf("%s: %d\n", hashtable->table[k]->key ,type_v->base_type);

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






/* /////
void codegen(struct tree *parseT){


  int i, j;
  if (parseT==NULL) return;

  /*
   * this is a post-order traversal, so visit children first
   */


/* ////////
  for(i=0;i<parseT->nkids;i++)
     codegen(parseT->kids[i]);

  /*
   * back from children, consider what we have to do with
   * this node. The main thing we have to do, one way or
   * another, is assign t->code
   */

/* //////
  switch (parseT->prodrule) {
  case PLUS: {
     parseT->code = concat(parseT->kids[0].code, parseT->kids[1].code);
     g = gen(PLUS, parseT->address,
             parseT->kids[0].address, parseT->kids[1].address);
     parseT->code = concat(parseT->code, g);
     break;
     }
  /*
   * ... really, a bazillion cases, up to one for each
   * production rule (in the worst case)
   */

/* ////////
  default:
     /* default is: concatenate our children's code */
/* ///////
     parseT->code = NULL;
     for(i=0;i<t->nkids;i++)
        parseT->code = concat(parseT->code, parseT->kids[i].code);
  }


}
*/
