#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

/*
 * Allocate memory for a node of type struct nodeStruct and initialize
 * it with the value size. Return a pointer to the new node.
 */
struct nodeStruct* List_createNode(int size, void *address){
	struct nodeStruct *new = NULL;
	new = (struct nodeStruct*)malloc(sizeof(struct nodeStruct));
	if(new != NULL){
		new->size = size;
		new->address = address;
		new->next = NULL;
	}
	return new;
} 

/*
 * Insert node at the head of the list.
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
 * Count number of nodes in the list.
 * Return 0 if the list is empty, i.e., head == NULL
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
 * Return the first node holding the value size, return NULL if none found
 */
struct nodeStruct* List_findNode(struct nodeStruct *head, void *address){
	if(head == NULL){
		return NULL;
	}
	else{
		struct nodeStruct *current = head;
		while(current != NULL){
			if(current->address == address){
				return current;
			}
			current = current->next;
		}
		return NULL;
	}
} 

/*
 * Delete node from the list and free memory allocated to it.
 * This function assumes that node has been properly set (by for example
 * calling List_findNode()) to a valid node in the list. If the list contains
 * only node, the head of the list should be set to NULL.
 */
void List_deleteNode (struct nodeStruct **headRef, struct nodeStruct *node){
	struct nodeStruct *current = *headRef;
	if(current->size == node->size){
		*headRef = current->next;
		free(current);
	}
	else{
		while(current->next->size != node->size){
			current = current->next;
		}
		struct nodeStruct *nodeDelete = current->next;
		current->next = current->next->next;
		free(nodeDelete);
	}
} 


/*
 * Sort the list in ascending order based on the address field.
 * Any sorting algorithm is fine.
 * This algorithm was taken from https://www.geeksforgeeks.org/insertion-sort-for-singly-linked-list/
 */
void List_sort (struct nodeStruct **headRef){
    struct nodeStruct *sorted = NULL; 
    struct nodeStruct *current = *headRef; 
    while (current != NULL) 
    { 
        struct nodeStruct *next = current->next; 
        sortedInsert(&sorted, current); 
        current = next; 
    } 
    *headRef = sorted; 
}

void sortedInsert(struct nodeStruct** headRef, struct nodeStruct* new_node) { 
    struct nodeStruct* current = *headRef; 
    if (*headRef == NULL || (*headRef)->address >= new_node->address){ 
        new_node->next = *headRef; 
        *headRef = new_node; 
    } 
    else{ 
        while(current->next != NULL && current->next->address < new_node->address){
        	current = current->next; 
        } 
        new_node->next = current->next; 
        current->next = new_node; 
    } 
} 