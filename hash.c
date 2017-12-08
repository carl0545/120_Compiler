/*
*Altered code from C Hash table from https://gist.github.com/tonious/1377667
*/

#include "hash.h"
#include "string.h"

/*
*Hash function for a string
*/
struct hashtable_s* ht_create(int size, struct hashtable_s *par){

  	struct hashtable_s *hashtable = NULL;
  	int i;

    if(size < 1){
      fprintf(stderr, "Can't have a hashtable of size less than 1\n");
      return NULL;
    }

  	/* Allocate the table itself. */
  	if( ( hashtable = malloc( sizeof( struct hashtable_s ) ) ) == NULL ) {
      fprintf(stderr, "Error mallocing new hashtable\n");
  		return NULL;
  	}

    if(par != NULL){
      hashtable->parent = par;
    }
    else{
      hashtable->parent = NULL;
    }
  	/* Allocate pointers to the head nodes. */
  	if( ( hashtable->table = malloc( sizeof( struct entry_s * ) * size ) ) == NULL ) {
        fprintf(stderr, "Error mallocing new hashtable table\n");
  		return NULL;
  	}
  	for( i = 0; i < size; i++ ) {
  		hashtable->table[i] = NULL;
  	}

  	hashtable->size = size;

  	return hashtable;
}

/*
*Hash function for a string
*/
int ht_hash(struct hashtable_s *hashtable, char *key){


  	unsigned long int hashval = 0;
  	int i = 0;
    //fprintf(stderr, "INSIDE HASHT KEY: %s\n", key);

  	/* Convert our string to an integer */
  	while( hashval < ULONG_MAX && i < strlen( key ) ) {
      if(hashval >= ULONG_MAX) fprintf(stderr, "ERROR HASH\n");
  		hashval = hashval << 8;
  		hashval += key[ i ];
  		i++;
  	}

  	return hashval % hashtable->size;
}
/*
*Create a new entry node
*/
struct entry_s *ht_newpair(char *key, struct type120 *value){

  struct entry_s *newpair;

	if( ( newpair = malloc( sizeof( struct entry_s ) ) ) == NULL ) {
    fprintf(stderr, "Error mallocing new pair\n");
		return NULL;
	}

	if( ( newpair->key = strdup( key ) ) == NULL ) {
    fprintf(stderr, "Error mallocing new pair: key\n");
		return NULL;
	}

	newpair->value = value;

    //printf("Error mallocing new pair: value\n");
		//return NULL;


	newpair->next = NULL;

	return newpair;

}
/*
*Insert an entry into the hash table
*/
void ht_set(struct hashtable_s *hashtable, char *key, struct type120 *value){
  int bin = 0;
	struct entry_s *newpair = NULL;
	struct entry_s *next = NULL;
	struct entry_s *last = NULL;


  if(hashtable->parent != NULL){
    //printf("HASH SET: not null\n");
    if(ht_get(hashtable->parent, key) != NULL){
      fprintf(stderr, "Semantic Error: Cannot redeclare a variable made in global scope\n");
      exit(3);
    }
  }
  else{
    //printf("HASH SET: null\n");
  }

	bin = ht_hash(hashtable, key);
  //printf("HT_SET: hash function returned %d\n", bin);

	next = hashtable->table[ bin ];

	while( next != NULL && next->key != NULL && strcmp( key, next->key ) > 0 ) {
		last = next;
		next = next->next;
	}

	/* There's already a pair.  Let's replace that string. */
	if( next != NULL && next->key != NULL && strcmp( key, next->key ) == 0 ) {
    fprintf(stderr, "Semantic Error: Cannot redeclare a variable\n");
    exit(3);
		//free( next->value );
		//next->value = strdup( value );

	/* Nope, could't find it.  Time to grow a pair. */
	} else {
		newpair = ht_newpair( key, value );

		/* We're at the start of the linked list in this bin. */
		if( next == hashtable->table[ bin ] ) {
      //printf("\nSTART\n");
			newpair->next = next;
			hashtable->table[ bin ] = newpair;

		/* We're at the end of the linked list in this bin. */
		} else if ( next == NULL ) {
      //printf("\nSTART\n");
			last->next = newpair;

		/* We're in the middle of the list. */
		} else  {
      //printf("\nSTART\n");
			newpair->next = next;
			last->next = newpair;
		}
	}
}


struct type120 *ht_get(struct hashtable_s *hashtable, char *key){
  int bin = 0;
	struct entry_s *pair;


	bin = ht_hash( hashtable, key );

  //printf("get hash function returns %d\n", bin);
	/* Step through the bin, looking for our value. */
	pair = hashtable->table[ bin ];

	while( pair != NULL && pair->key != NULL && strcmp( key, pair->key ) > 0 ) {
		pair = pair->next;
	}


	/* Did we actually find anything? */
	if( pair == NULL || pair->key == NULL || strcmp( key, pair->key ) != 0 ) {
    //printf("COULDN'T FIND IN HT_GET\n");
    if(pair == NULL){
      //printf("PAIR IS NULL\n");
    }
    else if(pair->key == NULL){
      //printf("PAIR->key IS NULL\n");
    }
    else{
      //printf("STRCMP error\n");
    }
    if(hashtable->parent != NULL){
      //printf("we got here\n");
      return ht_get(hashtable->parent, key);
    }
    else{
        //fprintf(stderr, "ht_get returning NULL\n");
		    return NULL;
    }

	} else {
		return pair->value;
	}


}

void ht_set_size(struct hashtable_s *hashtable, int local, int global, int param, enum regions_h currRegion){
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

    printf("%s: %d\n", hashtable->table[k]->key ,type_v->base_type);

    switch(type_v->base_type){
        case INT_T:
          type_v->place.region = currRegion;
          type_v->place.offset = getOffset(currRegion, &global, &local, &param, INT_SIZE);
        break;
        case DOUBLE_T:
          type_v->place.region = currRegion;
          type_v->place.offset = getOffset(currRegion, &global, &local, &param, DOUBLE_SIZE);
        break;
        case CHAR_T:
        type_v->place.region = currRegion;
        type_v->place.offset = getOffset(currRegion, &global, &local, &param, CHAR_SIZE);
        break;
        case BOOL_T:
        type_v->place.region = currRegion;
        type_v->place.offset = getOffset(currRegion, &global, &local, &param, INT_SIZE);
        break;
        case ARRAY_T:
        break;
        case FUNCTION_T:
          printf("GOING INTO FUNCTION\n");
          ht_set_size(type_v->u.function.sources, local, global, param, LOCAL_H);
          printf("COMING OUT OF FUNCTION");

        break;
        case CLASS_T:
          printf("going into private:\n\n");
          ht_set_size(type_v->u.class.private, local, global, param, LOCAL_H);
          printf("coming out of private\n\n");
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

      switch(type_w->base_type){
          case INT_T:
            type_v->place.region = currRegion;
            type_v->place.offset = getOffset(currRegion, &global, &local, &param, INT_SIZE);
          break;
          case DOUBLE_T:
            type_v->place.region = currRegion;
            type_v->place.offset = getOffset(currRegion, &global, &local, &param, DOUBLE_SIZE);
          break;
          case CHAR_T:
          type_v->place.region = currRegion;
          type_v->place.offset = getOffset(currRegion, &global, &local, &param, CHAR_SIZE);
          break;
          case BOOL_T:
          type_v->place.region = currRegion;
          type_v->place.offset = getOffset(currRegion, &global, &local, &param, INT_SIZE);
          break;
          case ARRAY_T:
          break;
          case FUNCTION_T:
          break;
          case CLASS_T:
            printf("going into private:\n\n");
            ht_set_size(type_v->u.class.private, local, global, param, LOCAL_H);
            printf("coming out of private\n\n");
          break;

          default:
            fprintf(stderr, "Error  in switch statement of ht_set_size, base_type: %d\n", type_v->base_type);
            exit(3);

      }

    }



  }


}

int getOffset(enum regions_h currRegion, int *global, int *local, int *param, int increment){
  int retVal;
  switch(currRegion){
    case GLOBAL_H:
      retVal = *global;
      *global += increment;
      break;
    case LOCAL_H:
      retVal = *local;
      *local += increment;
      break;
    case PARAM_H:
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

    //

    if(type_v->base_type == CLASS_T){
      printf("NULL VARIABLE: %s with basetype: %d\n", entry_v->key, type_v->base_type);
    }
    else if(type_v->base_type == FUNCTION_T){
      printSize(type_v->u.function.sources);
    }
    else{
      printf("Variable: %s has region %d with offset %d\n", entry_v->key, type_v->place.region, type_v->place.offset);
    }


  }



}
