#include "list.h"
/*
*add node to list
*/
void add(struct listnode **head, struct type120 *type_a){
  struct listnode *temp;
  struct listnode *traverse;
  temp = (struct listnode *)malloc(sizeof(struct listnode));
  temp->type = type_a;
  temp->next = NULL;

  traverse = *head;

  if(*head == NULL){
    *head = temp;
    return;
  }

  while(traverse->next != NULL){

    traverse = traverse->next;
  }

  traverse->next = temp;

}

/*
*print out the elements within a list
*/
void printList(struct listnode *head){
  struct listnode *traverse;
  traverse = head;
  int count = 1;

  traverse = traverse->next; //error probs
  while(traverse != NULL){
    printf(" type: %d", traverse->type->base_type);
    count++;
    traverse = traverse->next;
  }

}

int listSize(struct listnode *head){
  struct listnode *traverse;
  traverse = head;
  int count = 0;

  traverse = traverse->next; //error probs
  while(traverse != NULL){
    count++;
    traverse = traverse->next;
  }

  return count;
}

struct type120* listGet(struct listnode *head, int elem){
  struct listnode *traverse, *prev;
  traverse = head;

  traverse = traverse->next;

  for(int k = elem; k > 0; k--){
    prev = traverse;
    traverse = traverse->next;
  }

  return prev->type;
}
