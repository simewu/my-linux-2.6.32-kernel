#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/pid.h>

asmlinkage int sys_printother(int p_id) {
	struct task_struct* task;

	struct pid* pid_struct = find_get_pid(p_id);
	task = pid_task(pid_struct, PIDTYPE_PID); //get the task

	printk(KERN_EMERG "%d Printing task information for current task:\n", p_id);
	printk(KERN_EMERG "%s [%d] [%ld]\n", task -> comm, task -> pid, task -> state);

	printk(KERN_EMERG "Printing task list from curent task:\n");
	while(task != &init_task) {
		task = task -> parent;
		printk(KERN_EMERG "%s %d %ld\n", task -> comm, task -> pid, task -> state);
	}

    	printk(KERN_EMERG "PRINTOTHER COMPLETED\n");
	return 0;
}
