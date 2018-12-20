#include <linux/kernel.h>
#include <linux/sched.h>

asmlinkage int sys_helloworld (void)
{
	printk(KERN_EMERG, "Hello, World!\n");
	return 0;
}
