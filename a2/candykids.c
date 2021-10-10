#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "bbuff.h"
#include "stats.h"

_Bool stop_thread = false;

//data type for Candy
typedef struct{
    int factory_number;
    double creation_ts_ms; //use current_time_in_ms to grab time item was created
} candy_t;

//used to track when the candy was created
double current_time_in_ms(void) {
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);
    return now.tv_sec * 1000.0 + now.tv_nsec/1000000.0;
}

void *newFactory(void *id){
	int factoryId = *((int *)id);
	printf("Factory %d created\n", factoryId);
	while(!stop_thread){
		int wait = rand() % 4;
		printf("Factory %d ships candy & waits %ds\n", factoryId, wait);
		candy_t *candy = malloc(sizeof(candy_t));
		candy->factory_number = factoryId;
		candy->creation_ts_ms = current_time_in_ms();
		stats_record_produced(factoryId);
		bbuff_blocking_insert(candy);
		sleep(wait);
	}
	printf("Candy-factory %d done\n",factoryId);
	free(id);
	return NULL;
}

void *newKid(){
	while(true){
		int wait = rand() % 2;
		candy_t *candy = bbuff_blocking_extract();
		printf("Eating candy from factory %d\n",candy->factory_number);
		stats_record_consumed(candy->factory_number, current_time_in_ms() - candy->creation_ts_ms);
		free(candy);
		sleep(wait);
	}
	return NULL;
}

int main(int argc, char* argv[]){
	//extract arguments and check they're proper input
	if(argc != 4){
		write(STDOUT_FILENO, "ERROR: Wrong number of arguments. Need 3\n", strlen("ERROR: Wrong number of arguments. Need 3\n"));
		return -1;
	}

	int values[3]; // factories | kids | factory run time
	for(int i = 0; i < 3; i++){
		values[i] = atoi(argv[i+1]);
		if(values[i] <= 0){
			write(STDOUT_FILENO, "ERROR: Arguments must be greater than 0\n", strlen("ERROR: Arguments must be greater than 0\n"));
			return -1;
		}
	}

	//initialize modules
	bbuff_init();
	stats_init(values[0]);
	pthread_t *factoryIDs = malloc(sizeof(pthread_t)*values[0]);
	pthread_t *kidIDs = malloc(sizeof(pthread_t)*values[1]);

	//lauch candy-factory threads
	for(int i = 0; i < values[0]; i++){
		pthread_t factoryThreadId;
		int *id = malloc(sizeof(*id));
		*id = i;
		if(pthread_create(&factoryThreadId, NULL, newFactory, id) != 0){
			write(STDOUT_FILENO, "ERROR: Failed to create factory\n", strlen("ERROR: Failed to create factory\n"));
			return -1;
		}
		factoryIDs[i] = factoryThreadId;
	}

	//lauch kid threads
	for(int i = 0; i < values[1]; i++){
		pthread_t kidThreadId;
		if(pthread_create(&kidThreadId, NULL, newKid, NULL) != 0){
			write(STDOUT_FILENO, "ERROR: Failed to create factory\n", strlen("ERROR: Failed to create factory\n"));
			return -1;
		}
		kidIDs[i] = kidThreadId;
	}

	//wait for requested time
	for(int i = 0; i < values[2]; i++){
		sleep(1);
		printf("Running time: %ds\n",i+1);
	}

	//stop factory threads
	stop_thread = true;
	for(int i = 0; i < values[0]; i++){
		pthread_join(factoryIDs[i], NULL);
	}

	//wait until no more candy
	while(!bbuff_is_empty()){
		write(STDOUT_FILENO, "Waiting for all candy to be consumed.....\n", strlen("Waiting for all candy to be consumed.....\n"));
		sleep(1);
	}

	//stop kid threads
	for(int i = 0; i < values[1]; i++){
		pthread_cancel(kidIDs[i]);
		pthread_join(kidIDs[i],NULL);
	}

	//print statistics
	stats_display(values[0]);

	//clean up memory
	free(factoryIDs);
	free(kidIDs);
	stats_cleanup();
	printf("Closing Program\n");
	return 0;
}