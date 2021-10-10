#include <stdio.h>
#include "Dlist.h"
#include <stdbool.h>
#include <string.h>
#include <assert.h>

/*Prints LL

	//Read head->tail
	current = head;
	while(current != NULL){
		printf("%d\n",current->item);
	 	current = current->next;
	}
	printf("Break\n");

	//Read tail->head
	current = tail;
	while(current != NULL){
		printf("%d\n",current->item);
	 	current = current->prev;
	}
	printf("Break\n");
*/
int main(int argc, char** argv)
{
	printf("Starting tests...\n");
	struct nodeStruct *head = NULL;
	struct nodeStruct *current = head;

	// Starting count:
	assert(Dlist_countNodes(head) == 0);

	// Create 2 nodes, Head/Head:
	struct nodeStruct* firstNode = Dlist_createNode(0);
	Dlist_insertHead(&head, firstNode);
	assert(Dlist_countNodes(head) == 1);

	struct nodeStruct* secondNode = Dlist_createNode(1);
	Dlist_insertHead(&head, secondNode);
	assert(Dlist_countNodes(head) == 2);

	struct nodeStruct *node = Dlist_findNode(head, 0);
	assert(node == firstNode);
	Dlist_deleteNode(&head, node);

	node = Dlist_findNode(head, 1);
	assert(node == secondNode);
	Dlist_deleteNode(&head, node);

	assert(Dlist_countNodes(head) == 0);

	// Create 2 nodes, Tail/tail:
	firstNode = Dlist_createNode(0);
	Dlist_insertTail(&head, firstNode);

	assert(Dlist_countNodes(head) == 1);

	firstNode = Dlist_createNode(1);
	Dlist_insertTail(&head, firstNode);
	assert(Dlist_countNodes(head) == 2);

	node = Dlist_findNode(head, 0);
	Dlist_deleteNode(&head, node);

	node = Dlist_findNode(head, 1);
	Dlist_deleteNode(&head, node);



	assert(Dlist_countNodes(head) == 0);

	// Create 2 nodes, Tail/Head:
	firstNode = Dlist_createNode(0);
	Dlist_insertTail(&head, firstNode);
	assert(Dlist_countNodes(head) == 1);

	firstNode = Dlist_createNode(-1);
	Dlist_insertHead(&head, firstNode);
	assert(Dlist_countNodes(head) == 2);

	node = Dlist_findNode(head, 0);
	Dlist_deleteNode(&head, node);
	node = Dlist_findNode(head, -1);
	Dlist_deleteNode(&head, node);

	assert(Dlist_countNodes(head) == 0);

	// Create list 2,-5,1,-2,0:
	firstNode = Dlist_createNode(2);
	Dlist_insertHead(&head, firstNode);
	assert(Dlist_countNodes(head) == 1);

	secondNode = Dlist_createNode(-5);
	Dlist_insertTail(&head, secondNode);
	assert(Dlist_countNodes(head) == 2);

	struct nodeStruct* thirdNode = Dlist_createNode(1);
	Dlist_insertTail(&head, thirdNode);
	assert(Dlist_countNodes(head) == 3);

	struct nodeStruct* fourthNode = Dlist_createNode(-2);
	Dlist_insertTail(&head, fourthNode);
	assert(Dlist_countNodes(head) == 4);

	struct nodeStruct* fifthNode = Dlist_createNode(0);
	Dlist_insertTail(&head, fifthNode);
	assert(Dlist_countNodes(head) == 5);

	assert(Dlist_findNode(head, -5) == secondNode);
	assert(Dlist_findNode(head, 3) == NULL);
	assert(Dlist_findNode(head, 1) == thirdNode);

	// //delete from middle of list
	node = Dlist_findNode(head, -2);
	Dlist_deleteNode(&head, node);

	//delete from tail
	node = Dlist_findNode(head, 0);
	Dlist_deleteNode(&head, node);

	//delete from head
	node = Dlist_findNode(head, 2);
	Dlist_deleteNode(&head, node);

	//List is currently -5,1

	node = Dlist_createNode(-9);
	Dlist_insertTail(&head, node);
	node = Dlist_createNode(0);
	Dlist_insertHead(&head, node);
	node = Dlist_createNode(3);
	Dlist_insertHead(&head, node);
	node = Dlist_createNode(2);
	Dlist_insertTail(&head, node);
	//List is currently 3,0,-5,1,-9,2

	// Sort and verify:
	Dlist_sort(&head);

	current = head;
	assert(current->item == -9);
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
	assert(current->item == 2);
	assert(current->next != NULL);
	current = current->next;
	assert(current->item == 3);
	assert(current->next == NULL);

	assert(Dlist_countNodes(head) == 6);

	node = Dlist_findNode(head, -9);
	Dlist_deleteNode(&head, node);
	node = Dlist_findNode(head, 3);
	Dlist_deleteNode(&head, node);
	node = Dlist_findNode(head, 0);
	Dlist_deleteNode(&head, node);
	node = Dlist_findNode(head, 1);
	Dlist_deleteNode(&head, node);
	node = Dlist_findNode(head, 2);
	Dlist_deleteNode(&head, node);
	node = Dlist_findNode(head, -5);
	Dlist_deleteNode(&head, node);

	assert(Dlist_countNodes(head) == 0);

	printf("\nExecution finished.\n");
	return 0;
}
