#include <asm/io.h>
#include<linux/fs.h> 
#include<linux/init.h>
#include<linux/kernel.h> 
#include<linux/kthread.h> 
#include<linux/list.h> 
#include<linux/mm.h> 
#include<linux/module.h>
#include<linux/proc_fs.h>
#include<linux/sched.h>
#include<linux/slab.h> 
#include<linux/types.h>

// Exchanges data between the user and kernel space via mmap
#define PAGE_SIZE 4096

static unsigned char *buffer;
static unsigned char array[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

struct proc_dir_entry *tempdir, *tempinfo;
static struct page *virt_to_page(void *kaddr);
static void allocate_memory(void);
static void clear_memory(void);
int my_map(struct file *filePointer, struct vm_area_struct *vma);

static const struct file_operations myproc_fops = {.mmap = my_map,};


static void allocate_memory(void){
    buffer = (unsigned char*) kmalloc(PAGE_SIZE, GFP_KERNEL);
    SetPageReserved(virt_to_page(buffer));
}

static void clear_memory(void){
    ClearPageReserved(virt_to_page(buffer));
    kfree(buffer);
}

static int my_map(struct file *filePointer, struct vm_area_struct *vma){
    unsigned long page;
    unsigned char i;
    unsigned long start = (unsigned long)vma->vm_start;
    unsigned long end =  (unsigned long)vma->vm_end;
    unsigned long size = (unsigned long)(end - start);
    page = virt_to_phys(buffer) >> PAGE_SHIFT; // Physical address
    if(remap_pfn_range(vma, start, page, size, PAGE_SHARED)) return -1; // Handle the vma
    for(i = 1; i <= 12; i++)
        buffer[i - 1] = array[i - 1]; // Set the data in the buffer
    return 0;
}

static int init_myproc_module(void){
    unsigned char i;
    printk("Successfully initialized myproc module!\n");
    allocate_memory();
    for(i = 1; i <= 12; i++)
        buffer[i - 1] = array[i - 1]; // Set the data in the buffer  
    return 0;
}

static void exit_myproc_module(void){
    clear_memory();
    remove_proc_entry("myinfo", tempdir); 
    remove_proc_entry("mydir", NULL); 
    printk("Successfully removed myproc module!\n");
}

module_init(init_myproc_module);
module_exit(exit_myproc_module);

MODULE_LICENSE("GPL");
