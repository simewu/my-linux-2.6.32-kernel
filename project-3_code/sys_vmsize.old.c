#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/mm_types.h>
#include <linux/fs.h>
#include <asm/page.h>
#include <asm/mman.h>

asmlinkage int sys_vma_stats(int pid)
{
struct task_struct *this_task;
struct mm_struct *this_mem;
struct vm_area_struct *all_vmas;
struct vm_area_struct this_vma;

int num_vmas;
unsigned long start=0;
int count=0;
int total_size=0;

this_task = find_task_by_vpid(pid);
this_mem = this_task->active_mm;
all_vmas = this_mem->mmap;
num_vmas = this_mem->map_count;
this_vma= *all_vmas;
start = this_vma.vm_start;

	for(this_vma = *all_vmas; count < num_vmas; count ++)
	{
	total_size += this_vma.vm_end - this_vma.vm_start;
	if(count != num_vmas-1){this_vma = *(this_vma.vm_next);}
	}
printk("Total virtual address space = %i\n",total_size);
return 0;
}
