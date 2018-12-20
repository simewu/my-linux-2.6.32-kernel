#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/module.h>

asmlinkage int sys_printself (void)
{
    struct task_struct *task;

    printk("Current task information: ");
    printk(KERN_EMERG "%s [%d] [%ld]\n", current -> comm, current -> pid, current -> state);

    // Trace the task back to init
    printk("Task list:");
    for(task = current; task != &init_task; task = task->parent)
    {
        printk(KERN_EMERG "%s [%d] [%ld]\n", task -> comm, task -> pid, task -> state);
    }

    printk(KERN_EMERG "PRINTSELF COMPLETED\n");
    return 0;
}
