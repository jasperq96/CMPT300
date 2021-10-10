#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <semaphore.h>
#include "myalloc.h"
#include "list.h"

sem_t mutex;

struct Myalloc {
    enum allocation_algorithm aalgorithm;
    int size;
    void* memory;
    // Some other data members you want, 
    // such as lists to record allocated/free memory
    struct nodeStruct *allocated; //LL of allocated memory slots
    struct nodeStruct *free; //LL of free memory slots
};

struct Myalloc myalloc;

void initialize_allocator(int _size, enum allocation_algorithm _aalgorithm) {
    assert(_size > 0);
    sem_init(&mutex, 0, 1);
    myalloc.aalgorithm = _aalgorithm;
    myalloc.size = _size;
    myalloc.memory = malloc((size_t)myalloc.size);

    // Add some other initialization 
    memset(myalloc.memory, 0, myalloc.size);
    myalloc.free = List_createNode(myalloc.size, myalloc.memory);
}

void destroy_allocator() {
    free(myalloc.memory);
    struct nodeStruct *allocated = myalloc.allocated;
    struct nodeStruct *freeNode = myalloc.free;

    while(allocated != NULL) {
        struct nodeStruct *node = allocated;
        allocated = allocated->next;
        free(node);
    }

    while(freeNode != NULL) {
        struct nodeStruct *node = freeNode;
        freeNode = freeNode->next;
        free(node);
    }

    // free other dynamic allocated memory to avoid memory leak
}

void* allocate(int _size) {
    void* ptr = NULL;

    // Allocate memory from myalloc.memory 
    // ptr = address of allocated memory
    // based on algoirthm
    struct nodeStruct *allocated = myalloc.allocated;
    struct nodeStruct *free = myalloc.free;
    sem_wait(&mutex);
    if(free == NULL){
     return NULL;
    }
    if(myalloc.aalgorithm == FIRST_FIT){
        while(free->size < _size){
            if(free->next != NULL){
                free = free->next;
            }
            else{
                printf("There is no free memory slot large enough to allocate.\n");
                return NULL;
            }
        }
    }
    else if(myalloc.aalgorithm == BEST_FIT){
        int smallest_fit = myalloc.size + 1;
        void *address_of_smallest = NULL;
        while(free != NULL){
            if(free->size < smallest_fit && free->size >= _size){
                address_of_smallest = free->address;
                smallest_fit = free->size;
            }
            free = free->next;
        }
        if(address_of_smallest == NULL){
            return ptr;
        }
        free = List_findNode(myalloc.free, address_of_smallest);
    }
    else{ //worst fit algorithm
        int worst_fit = -1;
        void *address_of_worst = NULL;
        while(free != NULL){
            if(free->size > worst_fit && free->size >= _size){
                address_of_worst = free->address;
                worst_fit = free->size;
            }
            free = free->next;
        }

        if(address_of_worst == NULL){
            return ptr;
        }
        free = List_findNode(myalloc.free, address_of_worst);
    }

    struct nodeStruct *newAlloc = List_createNode(_size, free->address);
    if(allocated == NULL){
        myalloc.allocated = newAlloc;
    }
    else{
        List_insertHead(&myalloc.allocated, newAlloc);
        List_sort(&myalloc.allocated);
    }

    ptr = newAlloc->address;
    if(free->size == _size){ //if the free spot is exactly _size, we can remove it from the list
        List_deleteNode(&myalloc.free, free);
    }
    else{
        //partition the free space and update information if _size is smaller than free space.
        free->address += _size;
        free->size -= _size;
    }
    sem_post(&mutex);
    return ptr;
}

void deallocate(void* _ptr) {
    assert(_ptr != NULL);
    struct nodeStruct *allocated = List_findNode(myalloc.allocated, _ptr);
    struct nodeStruct *free = myalloc.free;
    sem_wait(&mutex);
    if(free == NULL) {
        free = List_createNode(allocated->size,_ptr);
        myalloc.free = free;
        List_deleteNode(&myalloc.allocated, allocated);
        return;
    }

    while(free->next != NULL) {
        if(free->address < _ptr && free->next->address < _ptr) {
            free = free->next;
        } 
        else{
            break;
        }
    }

    if (free->address < _ptr) {
        if((free->address + free->size) == _ptr) {
            free->size += allocated->size;
        } 
        else{
            struct nodeStruct *newFreeChunk = List_createNode(allocated->size,_ptr);
            if(newFreeChunk != NULL) {
                newFreeChunk->next = free->next;
                free->next = newFreeChunk;
            }
        }
    } 
    else{        
        struct nodeStruct *newFreeChunk = List_createNode(allocated->size,_ptr);
        newFreeChunk->next = free;
        myalloc.free = newFreeChunk;
    }

    free = myalloc.free;
    while(free->next != NULL) {
        if((free->address + free->size) == free->next->address) {
            free->size += free->next->size;
            List_deleteNode(&myalloc.free, free->next);
        }
        if(free->next != NULL) {            
            free = free->next;
        }
    }

    List_deleteNode(&myalloc.allocated, allocated);
    sem_post(&mutex);    
}

int compact_allocation(void** _before, void** _after) {
    int compacted_size = 0;

    // compact allocated memory
    // update _before, _after and compacted_size
    struct nodeStruct *current_allocated = myalloc.allocated;
    struct nodeStruct *current_free = myalloc.free;

    //all memory is currently free
    if (current_allocated == NULL) {
        return 0;
    }

    //all memory is currently taken
    sem_wait(&mutex);
    if (current_free == NULL) {
        while (current_allocated != NULL) {
            _after[compacted_size] = current_allocated->address;
            current_allocated = current_allocated->next;
            compacted_size++;
        }
        return compacted_size;
    }

    while (current_free != NULL && current_allocated != NULL) {
        if ((current_free->address + current_free->size) == current_allocated->address) { //if the free node is on the left of a allocated node
            _before[compacted_size] = current_allocated->address;
            _after[compacted_size] = current_free->address;
            current_allocated->address = current_free->address;
            current_free->address += current_allocated->size;
            current_allocated = current_allocated->next;
            compacted_size++;
        } 
        else{
             if (current_free->next == NULL) {
                break;
            } 
            else if((current_free->address + current_free->size) == current_free->next->address) {//if free node is on the left of another free node, combine the two, delete the second one.
                current_free->size += current_free->next->size;
                List_deleteNode(&myalloc.free, current_free->next);
            } 
            else if(current_allocated != NULL) { //if theres no free node on the left of my allocated node
                _after[compacted_size] = current_allocated->address;
                current_allocated = current_allocated->next;
                compacted_size++;
            }
        }
    }
    sem_post(&mutex);
    return compacted_size;
}

int available_memory() {
    int available_memory_size = 0;
    // Calculate available memory size
    struct nodeStruct *current = myalloc.free;
    while(current != NULL){
        available_memory_size += current->size;
        current = current->next;
    }
    return available_memory_size;
}

void print_statistics() {
    int allocated_size = 0;
    int allocated_chunks = 0;
    int free_size = 0;
    int free_chunks = 0;
    int smallest_free_chunk_size = myalloc.size;
    int largest_free_chunk_size = 0;

    // Calculate the statistics
    struct nodeStruct *allocated = myalloc.allocated;
    struct nodeStruct *free = myalloc.free;

    while(allocated != NULL){
     allocated_size += allocated->size;
     allocated_chunks += 1;
     allocated = allocated->next;
    }

    if(free == NULL){
     smallest_free_chunk_size = 0;
    }
    else{
     while(free != NULL){
         free_size += free->size;
         free_chunks += 1;
         if(free->size < smallest_free_chunk_size){
             smallest_free_chunk_size = free->size;
         }
         if(largest_free_chunk_size < free->size){
             largest_free_chunk_size = free->size;
         }
         free = free->next;
     }
    }

    printf("Allocated size = %d\n", allocated_size);
    printf("Allocated chunks = %d\n", allocated_chunks);
    printf("Free size = %d\n", free_size);
    printf("Free chunks = %d\n", free_chunks);
    printf("Largest free chunk size = %d\n", largest_free_chunk_size);
    printf("Smallest free chunk size = %d\n", smallest_free_chunk_size);
}