#include "array_stats.h"
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/uaccess.h>

#define _ARRAY_STATS_ 437

asmlinkage long sys_array_stats(struct array_stats *stats, long *data, long size){
	long *copy_array = NULL;
	struct array_stats *results = NULL;
	long sum = 0, min = 0, max = 0;

	if(size <= 0){
		return -EINVAL;
	}

	copy_array = kmalloc(size * sizeof(long), GFP_KERNEL);
	for(int i = 0; i < size; i++){
		if(copy_from_user(&copy_array[i], &data[i], sizeof(long))){
			kfree(copy_array);
			return -EFAULT;
		}
	}


	min = max = data[0];
	for(int x = 0; x < size; x++){
		sum += data[x];
		if(data[x] < min){
			min = data[x];
		}
		if(data[x] > max){
			max = data[x];
		}
	}

	results = kmalloc(sizeof(struct array_stats), GFP_KERNEL);
	results->min = min;
	results->max = max;
	results->sum = sum;

	if(copy_from_user(stats, results, sizeof(struct array_stats))){
		kfree(copy_array);
		kfree(results);
		return -EFAULT;
	}

	kfree(copy_array);
	kfree(results);
	return 0;
}