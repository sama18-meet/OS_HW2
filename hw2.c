#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/sched.h>

asmlinkage long sys_hello(void) {
	printk("Hello, World!\n");
	return 0;
}

asmlinkage int sys_set_weight(int weight) {
	if (weight < 0) {
		return -EINVAL;	
	}
	current->proc_weight = weight;
	return 0;	
}

asmlinkage int sys_get_weight(void) {
	return current->proc_weight;
}
