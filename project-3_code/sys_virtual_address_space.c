#include <asm/mman.h>
#include <asm/page.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/mm_types.h>
#include <linux/sched.h>

asmlinkage int sys_virtual_address_space(int pid){
	struct task_struct *task;
	struct mm_struct *memory;
	struct vm_area_struct *all_vmas;
	struct vm_area_struct p_vma;
	
	int counter = 0;
	int total_count = 0;
	int total_size = 0;
	
	task = find_task_by_vpid(pid);
	memory = task -> active_mm;
	all_vmas = memory -> mmap;
	total_count = memory -> map_count;
	printk("Total map count = %d\n", total_count);

	for(p_vma = *all_vmas; counter < total_count; counter++){
		total_size += p_vma.vm_end - p_vma.vm_start;
		if(total_count != counter + 1) {
			p_vma = *(p_vma.vm_next);
		}
	}
	printk("Counter = %d\n", counter);
	printk("The total VMA size for pid = %d is %d\n", pid, total_size);
	return 0;
}
