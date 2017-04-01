///////////////////////////////////////////////////////////////////////
// Doubly Linked List
//
// This file should contain you implementation of the doubly-linked
// list data structure as described in the project documentation.
// 
// Some functions are provided for you - use these as guides for the
// implementation of the remaining functions.
//
// This code can be tested using the testbench module. See
// testbench.h in the source, and LL_testbench_documentation.pdf in 
// the project documentation for more information.
//
// GEORGIA INSTITUTE OF TECHNOLOGY, FALL 2016
///////////////////////////////////////////////////////////////////////


#include <stdlib.h>
#include <stdio.h>
#include "doubly_linked_list.h"

/**
 * create_llnode
 *
 * Helper function that creates a node by allocating memory for it on the heap,
 * and initializing its previous and next pointers to NULL and its data pointer to the input
 * data pointer
 *
 * @param data A void pointer to data the user is adding to the doublely linked list.
 * @return A pointer to the linked list node
 */
static LLNode* create_llnode(void* data) {
    LLNode* newNode = (LLNode*)malloc(sizeof(LLNode));
    newNode->data = data;
    newNode->previous = NULL;
    newNode->next = NULL;
    return newNode;
}

/**
 * create_dlinkedlist
 *
 * Creates a doublely liked list by allocating memory for it on the heap. Initialize the size to zero,
 * as well as head, current, and tail pointer to NULL
 *
 * @return A pointer to an empty dlinkedlist
 */
DLinkedList* create_dlinkedlist(void) {
    DLinkedList* newList = (DLinkedList*)malloc(sizeof(DLinkedList));
    newList->head = NULL;
    newList->tail = NULL;
    newList->current = NULL;
    newList->size = 0;
    return newList;
}

void insertHead(DLinkedList* dll, void* data){
  LLNode* newNode = create_llnode(data);
  if(dll->head == NULL){
    dll->size++;
    dll->head = newNode;
    dll->tail = newNode;
  }else{
    dll->size++;
    newNode->next = dll->head;
    (dll->head)->previous = newNode;
    dll->head = newNode;
  }
}


void insertTail(DLinkedList* dll, void* data){
    LLNode* newNode = create_llnode(data);
    if (dll->tail == NULL) {
        dll->size++;
        dll->head = newNode;
        dll->tail = newNode;
    } else {
        dll->size++;
        newNode->previous = dll->tail;
        (dll->tail)->next = newNode;
        dll->tail = newNode;
    }
}

int insertAfter(DLinkedList* dll, void* newData){
    LLNode* newNode = create_llnode(newData);
    if (dll->current == NULL || dll == NULL) {
        return 0;
    } else if (dll->current == dll->tail) {
        insertTail(dll, newData);
        return 1;
    } else {
        dll->size++;
        newNode->next = (dll->current)->next;
        newNode->previous = dll->current;
        ((dll->current)->next)->previous = newNode;
        (dll->current)->next = newNode;
        return 1;
    }
}

int insertBefore(DLinkedList* dll, void* newData){
  LLNode* newNode = create_llnode(newData);
  if (dll->current == NULL || dll == NULL) {
      return 0;
  } else if (dll->current == dll->head) {
      insertHead(dll, newData);
      return 1;
  } else {
      dll->size++;
      newNode->next = dll->current;
      newNode->previous = (dll->current)->previous;
      ((dll->current)->previous)->next = newNode;
      (dll->current)->previous = newNode;
      return 1;
  }
}

void* deleteBackward(DLinkedList* dll, int shouldFree){
  LLNode* temp = dll->current;
  if (temp == NULL || dll == NULL) {
      return NULL;
  } else if (dll->size == 1) {
      dll->current = NULL;
      dll->head = NULL;
      dll->tail = NULL;
      dll->size--;
      if (shouldFree) {
          free(temp->data);
      }
      free(temp);
      return NULL;
  } else if (temp == dll->head) {
      dll->head = temp->next; // move head pointer
      (dll->head)->previous = NULL; // new head have no previous pointer
      dll->current = NULL;
      if (shouldFree) {
          free(temp->data);
      }
      free(temp);
      dll->size--;
      return NULL;
  } else if (temp == dll->tail) {
      dll->tail = temp->previous; // move tail
      (dll->tail)->next = NULL; // pointer after tail is null
      dll->current = dll->tail; // new current
      if(shouldFree) {
          free(temp->data);
      }
      free(temp);
      dll->size--;
      return((dll->current)->data);
  } else {
      (temp->next)->previous = temp->previous;
      (temp->previous)->next = temp->next;
      dll->current = temp->previous;
      if(shouldFree) {
          free(temp->data);
      }
      free(temp);
      dll->size--;
      return((dll->current)->data);
  }
}

void* deleteForward(DLinkedList* dll, int shouldFree){
  LLNode* temp = dll->current;
  if (temp == NULL || dll == NULL) {
      return NULL;
  } else if (dll->size == 1) {
      dll->current = NULL;
      dll->head = NULL;
      dll->tail = NULL;
      if (shouldFree) {
          free(temp->data);
      }
      free(temp);
      dll->size--;
      return NULL;
  } else if (temp == dll->head) {
        dll->head = temp->next; // move head
        (dll->head)->previous = NULL; // set new head prev to null
        dll->current = dll->head; // move current
        if (shouldFree) {
            free(temp->data);
        }
        free(temp);
        dll->size--;
        return (dll->current)->data;
  } else if (temp == dll->tail) {
        dll->tail = temp->previous;
        (dll->tail)->next = NULL;
        dll->current = NULL;
        if (shouldFree) {
            free(temp->data);
        }
        free(temp);
        dll->size--;
        return NULL;
  } else {
        ((dll->current)->next)->previous = (dll->current)->previous;
        ((dll->current)->previous)->next = (dll->current)->next;
        dll->current = (dll->current)->next;
        if (shouldFree) {
            free(temp->data);
        }
        free(temp);
        dll->size--;
        return (dll->current)->data;
  }
}

void destroyList(DLinkedList* dll, int shouldFree){
  if(dll->head != NULL){
    getHead(dll);
    while(deleteForward(dll,shouldFree)){};
  }
  free(dll);
}

void* getHead(DLinkedList* dll){
  if(dll->head != NULL){
    dll->current = dll->head;
    return (dll->head)->data;
  }else{
    return NULL;
  }
}

void* getTail(DLinkedList* dll){
  if(dll->tail != NULL) {
    dll->current = dll->tail;
    return (dll->tail)->data;
  } else {
      return NULL;
  }
}

void* getCurrent(DLinkedList* dll){
  if(dll->current != NULL) {
      return (dll->current)->data;
  } else {
      return NULL;
  }
}

void* getNext(DLinkedList* dll){
  if((dll->current)->next != NULL) {
      dll->current = (dll->current)->next;
      return (dll->current)->data;
  } else {
      return NULL;
  }
}

void* getPrevious(DLinkedList* dll){
  if((dll->current)->previous != NULL) {
      dll->current = (dll->current)->previous;
      return (dll->current)->data;
  } else {
      return NULL;
  }
}

int getSize(DLinkedList* dll){
  return dll->size;
}