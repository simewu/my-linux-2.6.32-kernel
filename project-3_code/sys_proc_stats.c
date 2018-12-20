#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/mm_types.h>
#include <linux/mm.h>
#include <linux/highmem.h>
#include <asm/page.h>
#include <asm/pgtable.h>

// Process stats
asmlinkage int sys_proc_stats(unsigned long mem, int pid){
	spinlock_t *lock;
	printk("Spinlock_t created\n");
	int in_memory = 0;	
	pte_t *ptep; // Page table entry pointer
	pte_t pte; // Page table entry

	struct mm_struct *vma = find_task_by_vpid(pid) -> active_mm;
	printk("Found the process\n");
	// Page global directory
	pgd_t *pgd = pgd_offset(vma, mem);
	pud_t *pud = pud_offset(pgd, mem);
	pmd_t *pmd = pmd_offset(pud, mem);
	printk("Page global directory has been located\n");

	ptep = pte_offset_map_lock(vma, pmd, mem, &lock);
	pte = *ptep;
	printk("Page table offset has been found\n");
	in_memory = pte_present(pte); // Get the PRESENT bit
	
	printk("The present bit data [%s] in memory.\n", (in_memory ? " IS " : " IS NOT "));
	pte_unmap_unlock(ptep, lock);
	return 0;
}
