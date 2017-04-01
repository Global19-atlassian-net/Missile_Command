#ifndef DOUBLELINKEDLIST_H
#define DOUBLELINKEDLIST_H


/********************************************
 * Doublely Linked List library functions *
 * For more details on their functionality, *
 * see the project documentation *
 ********************************************/


/// The structure to store the information of a doubly linked list node
typedef struct llnode_t {
    void* data;
    struct llnode_t* previous;
    struct llnode_t* next;
}LLNode;


/// The structure to store the information of a doubly linked list
typedef struct dll_t {
    struct llnode_t* head;
    struct llnode_t* tail;
    struct llnode_t* current;
    int size;
} DLinkedList;


/**
 * create_dlinkedlist
 *
 * Creates a doublely liked list by allocating memory for it on the heap. Initialize the size to zero,
 * as well as head, current, and tail pointer to NULL
 *
 * @return A pointer to an empty dlinkedlist
 */
DLinkedList* create_dlinkedlist(void);


/**
 * InsertHead
 *
 * Insert the data to the head of the doublely linked list. Update the current pointer of the list only 
 * when it is originally NULL.
 *
 * @param dll A pointer to the doubly linked list
 * @param data A void pointer to data the user is adding to the doublely linked list.
 * 
 */
void insertHead(DLinkedList* dll, void* data);


/**
 * insertTail
 *
 * Insert the data to the tail of the doublely linked list. Update the current pointer of the list only 
 * when it is originally NULL
 *
 * @param dll A pointer to the doubly linked list
 * @param data A void pointer to data the user is adding to the doublely linked list.
 * 
 */
void insertTail(DLinkedList* dll, void* data);


/**
 * insertAfter
 *
 * Insert the new data to the doublely linked list right after the current pointer
 *
 * @param dll A pointer to the doubly linked list
 * @param newData A void pointer to the new data that the user want to add after data
 * @return 1 if insert the new data successfully
 *         0 if the current pointer is NULL
 */
int insertAfter(DLinkedList* dll, void* newData);


/**
 * insertBefore
 *
 * Insert the new data to the doublely linked list right before the current pointer
 *
 * @param dll A pointer to the doubly linked list
 * @param newData A void pointer to the new data that the user want to add after data
 * @return 1 if insert the new data successfully
 *         0 if the current pointer is NULL
 */
int insertBefore(DLinkedList* dll, void* newData);


/**
 * deleteBackward
 *
 * Delete the node the current pointer is pointed at, and move the current pointer backwards.
 * Be aware that deleteBackward will cause problem if the current pointer is pointing 
 * at list head
 *
 * @param dll A pointer to the doubly linked list
 * @param shouldFree Flag. 1 indicates if data should be freed upon deletion of node.
 * @return the data of the new current pointer and NULL if the current pointer is NULL
 */
void* deleteBackward(DLinkedList* dll, int shouldFree);


/**
 * deleteForward
 *
 * Delete the node the current pointer is pointed at, and move the current pointer forwards.
 * Be aware that deleteForward will cause problem if the current pointer is pointing 
 * at list tail
 *
 * @param dll A pointer to the doubly linked list
 * @param shouldFree Flag. 1 indicates if data should be freed upon deletion of node.
 * @return the data of the new current pointer and NULL if the current pointer is NULL
 */
void* deleteForward(DLinkedList* dll, int shouldFree);


/**
 * destroyList
 *
 * Destroy the doublely linked list. Everything in the linked list including list structure,
 * nodes and data are all freed from the heap
 *
 * @param dll A pointer to the doubly linked list
 * @param shouldFree Flag. 1 indicates if data should be freed upon deletion of node.
 *
 */
void destroyList(DLinkedList* dll, int shouldFree);


/**
 * getHead
 *
 * Return the data contained in the head of the doublely linked list, and set the list current pointer
 * to head
 *
 * @param dll A pointer to the doubly linked list
 * @return the head data or  NULL if head == NULL
 */
void* getHead(DLinkedList* dll);


/**
 * getTail
 *
 * Return the data contained in the tail of the doublely linked list, and set the current pointer
 * to tail
 *
 * @param dll A pointer to the doubly linked list
 * @return the tail data or  NULL if tail == NULL
 */
void* getTail(DLinkedList* dll);


/**
 * getCurrent
 *
 * Return the data the current pointer is pointing at
 *
 * @param dll A pointer to the doubly linked list
 * @return the current data or NULL if current == NULL
 */
void* getCurrent(DLinkedList* dll);


/**
 * getNext
 *
 * Return the next data the current pointer is pointing at, and move the current pointer to next node
 *
 * @param dll A pointer to the doubly linked list
 * @return the next data or NULL if current == NULL
 */
void* getNext(DLinkedList* dll);


/**
 * getPrevious
 *
 * Return the previous data the current pointer is pointing at, and move the current pointer to 
 * previous node
 *
 * @param dll A pointer to the doubly linked list
 * @return the previous data or NULL if current == NULL
 */
void* getPrevious(DLinkedList* dll);


/**
 * getSize
 *
 * Return the size of the doubly linked list
 *
 * @param dll A pointer to the doubly linked list
 * @return  the size
 */
int getSize(DLinkedList* dll);
#endif

