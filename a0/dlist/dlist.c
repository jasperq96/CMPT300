#include "Dlist.h"
#include <stdio.h>
#include <stdlib.h>

/* 
* Allocate memory for a node of type struct nodeStruct and initialize 
* it with the value item. Return a pointer to the new node. 
*/ 
struct nodeStruct* Dlist_createNode(int item){
	struct nodeStruct *new = NULL;
	new = (struct nodeStruct*)malloc(sizeof(struct nodeStruct));
	new->item = item;
	new->next = NULL;
	new->prev = NULL;
	return new;
} 

/* 
 Insert node at the head of the list. 
 */ 
void Dlist_insertHead (struct nodeStruct **headRef, struct nodeStruct *node){
	struct nodeStruct *oldHead = *headRef;
	node->next = oldHead;
	if(oldHead != NULL){
		oldHead->prev = node;
	}
	*headRef = node;
} 

/* 
* Insert node after the tail of the list. 
*/ 
void Dlist_insertTail (struct nodeStruct **headRef, struct nodeStruct *node){
	if(*headRef == NULL){ //no nodes
		*headRef = node;
	}
	else{
		struct nodeStruct *lastNode = *headRef;
		while(lastNode->next != NULL){
			lastNode = lastNode->next;
		}
		lastNode->next = node;
		node->prev = lastNode;
	}
} 

/* 
* Count number of nodes in the list. * Return 0 if the list is empty, i.e., head == NULL 
*/ 
int Dlist_countNodes (struct nodeStruct *head){
	if(head == NULL){
		return 0;
	}
	else{
		int count = 1;
		struct nodeStruct *current = head->next;
		while(current != NULL){
			count++;
			current = current->next;
		}
		return count;
	}
} 

/* 
* Return the first node holding the value item, return NULL if none found 
*/ 
struct nodeStruct* Dlist_findNode(struct nodeStruct *head, int item){
	if(head == NULL){
		return NULL;
	}
	else{
		struct nodeStruct *current = head;
		while(current != NULL){
			if(current->item == item){
				break;
			}
			current = current->next;
		}
		return current;
	}
} 

/* 
* Delete node from the list and free memory allocated to it. 
* This function assumes that node has been properly set to a valid node 
* in the list. For example, the client code may have found it by calling 
* Dlist_findNode(). If the list contains only one node, the head of the list 
* should be set to NULL. 
*/ 
void Dlist_deleteNode (struct nodeStruct **headRef, struct nodeStruct *node){
	struct nodeStruct *nodeDelete = *headRef;
	if(nodeDelete->next != NULL){ //not a single node
		if(nodeDelete == node){	//head of the DLL is it
			nodeDelete->next->prev = NULL;
			*headRef = nodeDelete->next;
		}
		else{ //node is somewhere after the head
			nodeDelete = Dlist_findNode(*headRef, node->item);
			nodeDelete->prev->next = nodeDelete->next;
			if(nodeDelete->next != NULL){
				nodeDelete->next->prev = nodeDelete->prev;
			}
		}
	}
	else{
		*headRef = NULL;
	}
	free(nodeDelete);	
} 

/* 
* Sort the list in ascending order based on the item field. 
* Any sorting algorithm is fine. 
*/ 
void Dlist_sort (struct nodeStruct **headRef){
	struct nodeStruct *current = *headRef;
	struct nodeStruct *smallest = NULL;
	struct nodeStruct *temp = NULL;
	while(current->next != NULL){
		smallest = current;
		temp = current->next;
		while(temp != NULL){
			if(smallest->item > temp->item){
				smallest = temp;
			}
			temp = temp->next;
		}
		int swap = current->item;
		current->item = smallest->item;
		smallest->item = swap;
		current = current->next;
	}
}