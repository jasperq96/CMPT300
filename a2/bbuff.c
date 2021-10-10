#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "bbuff.h"

/*
sem_post increments
sem_wait decrements
Ordering of sem_wait in insert and extract matter. Should check buffer first before availability
*/

void *buffer[BUFFER_SIZE];
sem_t buff_avail;
sem_t numCandies;
sem_t empty;
int nextCandy;
int nextInsert;

void bbuff_init(void){
	sem_init(&buff_avail, 0, 1);
	sem_init(&numCandies, 0, 0);
	sem_init(&empty, 0, BUFFER_SIZE);
	nextCandy = 0;
	nextInsert = 0;
}

void bbuff_blocking_insert(void* item){
	sem_wait(&empty);
	sem_wait(&buff_avail);
	buffer[nextInsert] = item;
	nextInsert = (nextInsert + 1) % BUFFER_SIZE;
	sem_post(&numCandies);
	sem_post(&buff_avail);
	return;
}

void* bbuff_blocking_extract(void){
	sem_wait(&numCandies);
	sem_wait(&buff_avail);
	void *candy = buffer[nextCandy];
	nextCandy = (nextCandy + 1) % BUFFER_SIZE;
	sem_post(&empty);
	sem_post(&buff_avail);
	return candy;
}

_Bool bbuff_is_empty(void){
	int count = 0;
	sem_getvalue(&empty, &count);
	if(count == BUFFER_SIZE){
		return true;
	}
	return false;
}