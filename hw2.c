#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/sched.h>

asmlinkage long sys_hello(void) {
	printk("Hello, World!\n");
	return 0;
}

static int calc_leaf_children_total_weight(struct task_struct *curr_task)
{
	int total_leaf_weight = 0;
    struct list_head *child;
    if(list_empty(&curr_task->children)){
        return curr_task->proc_weight;
    }
    list_for_each(child, &curr_task->children) {
        total_leaf_weight += calc_leaf_children_total_weight(list_entry(child, struct task_struct, sibling));
    }
    return total_leaf_weight;
}


asmlinkage long sys_set_weight(int weight) {
	if (weight < 0) {
		return -EINVAL;	
	}
	current->proc_weight = weight;
	return 0;	
}

asmlinkage long sys_get_weight(void) {
	return current->proc_weight;
}

asmlinkage long sys_get_leaf_children_sum(void)
{
    if(list_empty(&current->children)){
        return -ECHILD;
    }
    return calc_leaf_children_total_weight(current);
}

asmlinkage long sys_get_heaviest_ancestor(void)
{
    int max_weight = current->proc_weight ;
    long max_pid = current->pid;
    struct task_struct* curr_parent = current->parent;
    while(curr_parent){
		if (curr_parent->pid==0)
		{
			break;
		}
        else if(curr_parent->proc_weight  > max_weight){
            max_weight = curr_parent->proc_weight ;
            max_pid = curr_parent->pid;
        }
         else if(curr_parent->proc_weight  == max_weight && curr_parent->pid <max_pid ){
            max_weight = curr_parent->proc_weight ;
            max_pid = curr_parent->pid;
        }        
        curr_parent = curr_parent->parent;
    }
    return max_pid;
}
