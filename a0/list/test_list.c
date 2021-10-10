#include <stdio.h>
#include "list.h"
#include <stdbool.h>
#include <string.h>
#include <assert.h>

/*Prints LL
	current = head;
	while(current != NULL){
		printf("%d\n",current->item);
	 	current = current->next;
	}
	printf("Break\n");
*/
int main(int argc, char** argv)
{
	printf("Starting tests...\n");
	struct nodeStruct *head = NULL;
	struct nodeStruct *current = head;

	// Starting count:
	assert(List_countNodes(head) == 0);

	// Create 1 node at Tail and delete:
	struct nodeStruct* firstNode = List_createNode(0);
	List_insertTail(&head, firstNode);
	assert(List_countNodes(head) == 1);
	assert(List_findNode(head, 0) == firstNode);
	assert(List_findNode(head, 1) == NULL);

	struct nodeStruct *nodeOf0 = List_findNode(head, 0);
	List_deleteNode(&head, nodeOf0);

	assert(List_countNodes(head) == 0);
	assert(List_findNode(head, 0) == NULL);

	// Create 1 node at Head and delete:
	firstNode = List_createNode(0);
	List_insertHead(&head, firstNode);
	assert(List_countNodes(head) == 1);
	assert(List_findNode(head, 0) == firstNode);
	assert(List_findNode(head, 1) == NULL);

	nodeOf0 = List_findNode(head, 0);
	List_deleteNode(&head, nodeOf0);

	assert(List_countNodes(head) == 0);
	assert(List_findNode(head, 0) == NULL);

	// Create list:
	firstNode = List_createNode(1);
	List_insertHead(&head, firstNode);
	assert(List_countNodes(head) == 1);
	assert(List_findNode(head, 1) == firstNode);
	assert(List_findNode(head, 0) == NULL);
	

	// Insert tail:
	struct nodeStruct* nextNode = List_createNode(-5);
	List_insertTail(&head, nextNode);
	assert(List_countNodes(head) == 2);
	assert(List_findNode(head, 1) == firstNode);
	assert(List_findNode(head, -5) == nextNode);
	assert(List_findNode(head, 0) == NULL);

	nextNode = List_createNode(-7);
	List_insertTail(&head, nextNode);
	nextNode = List_createNode(20);
	List_insertHead(&head, nextNode);
	nextNode = List_createNode(10);
	List_insertTail(&head, nextNode);
	nextNode = List_createNode(0);
	List_insertTail(&head, nextNode);

	
	// Verify list:
	// LL is 20,1,-5,-7,10,0

	current = head;
	assert(current->item != 1);
	assert(current->item == 20);
	assert(current->next != NULL);
	current = current->next;
	assert(current->item == 1);
	assert(current->next != NULL);
	current = current->next;
	assert(current->item == -5);
	assert(current->next != NULL);
	current = current->next;
	assert(current->item == -7);
	assert(current->next != NULL);
	current = current->next;
	assert(current->item == 10);
	assert(current->next != NULL);
	current = current->next;
	assert(current->item == 0);
	assert(current->next == NULL);


	// Sort and verify:
	List_sort(&head);
	current = head;
	assert(current->item == -7);
	assert(current->next != NULL);
	current = current->next;
	assert(current->item == -5);
	assert(current->next != NULL);
	current = current->next;
	assert(current->item == 0);
	assert(current->next != NULL);
	current = current->next;
	assert(current->item == 1);
	assert(current->next != NULL);
	current = current->next;
	assert(current->item == 10);
	assert(current->next != NULL);
	current = current->next;
	assert(current->item == 20);
	assert(current->next == NULL);

	// Delete
	assert(List_countNodes(head) == 6);

	nodeOf0 = List_findNode(head, -7);
	List_deleteNode(&head, nodeOf0);
	assert(List_countNodes(head) == 5);
	assert(List_findNode(head, -7) == NULL);

	nodeOf0 = List_findNode(head, 0);
	List_deleteNode(&head, nodeOf0);
	assert(List_countNodes(head) == 4);
	assert(List_findNode(head, 0) == NULL);

	nodeOf0 = List_findNode(head, 10);
	List_deleteNode(&head, nodeOf0);
	assert(List_countNodes(head) == 3);
	assert(List_findNode(head, 10) == NULL);

	nodeOf0 = List_findNode(head, 20);
	List_deleteNode(&head, nodeOf0);
	assert(List_countNodes(head) == 2);
	assert(List_findNode(head, 20) == NULL);

	nodeOf0 = List_findNode(head, -5);
	List_deleteNode(&head, nodeOf0);
	assert(List_countNodes(head) == 1);
	assert(List_findNode(head, -5) == NULL);

	nodeOf0 = List_findNode(head, 1);
	List_deleteNode(&head, nodeOf0);
	assert(List_countNodes(head) == 0);
	assert(List_findNode(head, 1) == NULL);

	assert(head == NULL);

	printf("\nExecution finished.\n");
	return 0;
}
