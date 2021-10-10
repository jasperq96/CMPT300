#ifndef Dlist_H   
#define Dlist_H

struct nodeStruct{
	int item;
	struct nodeStruct *next;
	struct nodeStruct *prev;
};

/* Function prototypes */

struct nodeStruct* Dlist_createNode(int item); 
void Dlist_insertHead (struct nodeStruct **headRef, struct nodeStruct *node); 
void Dlist_insertTail (struct nodeStruct **headRef, struct nodeStruct *node); 
int Dlist_countNodes (struct nodeStruct *head); 
struct nodeStruct* Dlist_findNode(struct nodeStruct *head, int item); 
void Dlist_deleteNode (struct nodeStruct **headRef, struct nodeStruct *node); 
void Dlist_sort (struct nodeStruct **headRef);

#endif