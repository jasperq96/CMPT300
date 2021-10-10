#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "stats.h"

typedef struct{
	int made;
	int eaten;
	double min_delay;
	double avg_delay;
	double max_delay;
}stats_t;

stats_t *statsArray;

void stats_init(int num_producers){
	statsArray = malloc(sizeof(stats_t)*num_producers);
	for(int i = 0; i < num_producers; i++){
		statsArray[i].made = 0;
		statsArray[i].eaten = 0;
		statsArray[i].min_delay = -1;
		statsArray[i].avg_delay = -1;
		statsArray[i].max_delay = -1;
	}
}

void stats_cleanup(void){
	free(statsArray);
}

void stats_record_produced(int factory_number){
	statsArray[factory_number].made += 1;
}

void stats_record_consumed(int factory_number, double delay_in_ms){
	statsArray[factory_number].eaten += 1;
	if(statsArray[factory_number].min_delay == -1){
		statsArray[factory_number].min_delay = delay_in_ms;
		statsArray[factory_number].avg_delay = delay_in_ms;
		statsArray[factory_number].max_delay = delay_in_ms;
	}else{
		if(statsArray[factory_number].min_delay > delay_in_ms){
			statsArray[factory_number].min_delay = delay_in_ms;
		}
		if(statsArray[factory_number].max_delay < delay_in_ms){
			statsArray[factory_number].max_delay = delay_in_ms;
		}
		statsArray[factory_number].avg_delay = ((statsArray[factory_number].avg_delay * (statsArray[factory_number].eaten-1)) + delay_in_ms)/statsArray[factory_number].eaten;
	}
}

void stats_display(int num_producers){
	printf("Statistics:\n");
	printf("%8s %8s %8s %16s %16s %16s\n", "Factory#", "Made", "Eaten", "Min Delay[ms]", "Avg Delay[ms]", "Max Delay[ms]");
	for(int i = 0; i < num_producers; i++){
		printf("%8d %8d %8d \t %10.5f \t %10.5f \t %10.5f\n", i, statsArray[i].made, statsArray[i].eaten, statsArray[i].min_delay, statsArray[i].avg_delay, statsArray[i].max_delay);
		if(statsArray[i].made != statsArray[i].eaten){
			printf("ERROR: Mismatch between number made and eaten");
		}
	}
}