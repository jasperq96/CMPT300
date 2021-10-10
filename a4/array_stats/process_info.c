#include <linux/sched.h>
#include<string.h>

struct process_info getInfo(struct task_struct* process);

struct process_info getInfo(struct task_struct* process){
	struct process_info result;
	struct list_head* ptr;

	result.pid = process->pid;
	strcpy(result.name, process->comm);
	result.state = process->state;
	result.uid = process->cred->uid.val;
	result.nvcsw = process->nvcsw;
	result.nivcsw = process->nivcsw;
	result.num_children = 0;
	result.num_siblings = 0;
	
	list_for_each(ptr, &(process->children)){
		result.num_children++;
	}
	list_for_each(ptr, &(process->sibling)){
		result.num_siblings++;
	}

	return result;
}