#include "list.h"
#include <stdio.h>
#include <stdlib.h>

/* 
* Allocate memory for a node of type struct nodeStruct and initialize 
* it with the value item. Return a pointer to the new node. 
*/ 
struct nodeStruct* List_createNode(int item){
	struct nodeStruct *new = NULL;
	new = (struct nodeStruct*)malloc(sizeof(struct nodeStruct));
	new->item = item;
	new->next = NULL;
	return new;
} 

/* 
 Insert node at the head of the list. 
 */ 
void List_insertHead (struct nodeStruct **headRef, struct nodeStruct *node){
	struct nodeStruct *oldHead = *headRef;
	node->next = oldHead;
	*headRef = node;
}

/* 
* Insert node after the tail of the list. 
*/ 
void List_insertTail (struct nodeStruct **headRef, struct nodeStruct *node){
	if(*headRef == NULL){
		*headRef = node;
	}
	else{
		struct nodeStruct *lastNode = *headRef;
		while(lastNode->next != NULL){
			lastNode = lastNode->next;
		}
		lastNode->next = node;
	}
} 

/* 
* Count number of nodes in the list. * Return 0 if the list is empty, i.e., head == NULL 
*/ 
int List_countNodes (struct nodeStruct *head){
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
struct nodeStruct* List_findNode(struct nodeStruct *head, int item){
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
* List_findNode(). If the list contains only one node, the head of the list 
* should be set to NULL. 
*/ 
void List_deleteNode (struct nodeStruct **headRef, struct nodeStruct *node){
	struct nodeStruct *current = *headRef;
	if(current->item == node->item){
		*headRef = current->next;
		free(current);
	}
	else{
		while(current->next->item != node->item){
			current = current->next;
		}
		struct nodeStruct *nodeDelete = current->next;
		current->next = current->next->next;
		free(nodeDelete);
	}
} 

/* 
* Sort the list in ascending order based on the item field. 
* Any sorting algorithm is fine. 
*/ 
void List_sort (struct nodeStruct **headRef){
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