#ifndef LIST_H   
#define LIST_H

struct nodeStruct{
	int size;
	void *address;
	struct nodeStruct *next;
};

/* Function prototypes */

struct nodeStruct* List_createNode(int size, void *address); 
void List_insertHead (struct nodeStruct **headRef, struct nodeStruct *node); 
void List_insertTail (struct nodeStruct **headRef, struct nodeStruct *node); 
int List_countNodes (struct nodeStruct *head); 
struct nodeStruct* List_findNode(struct nodeStruct *head, void *address); 
void List_deleteNode (struct nodeStruct **headRef, struct nodeStruct *node); 
void List_sort (struct nodeStruct **headRef);
void sortedInsert(struct nodeStruct** head_ref, struct nodeStruct* new_node);

#endif