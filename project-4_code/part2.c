#include <asm/uaccess.h>
#include <linux/kernel.h>
#include <linux/module.h> 
#include <linux/proc_fs.h> 
#include <linux/string.h>
#include <linux/vmalloc.h> 

#define MAX_LEN 4096
int read_info(char *page, char **start, off_t offset, int count, int *EOF, 
             void *data);
ssize_t write_info(struct file *filePointer, const char __user *buff, unsigned 
             long length, void *data);

static struct proc_dir_entry *proc_entry;
static char *info;
static int write_index; 
static int read_index;

int init_module(void) {
    int ret = 0;
    info = (char*) vmalloc(MAX_LEN); 
    memset(info, 0, MAX_LEN);
    proc_entry = create_proc_entry("test_proc", 0644, NULL);
    write_index = 0;
    read_index = 0;
    proc_entry->read_proc = read_info; 
    proc_entry->write_proc = write_info;
    printk(KERN_INFO "test_proc created .\n");
    return ret; 
}

void cleanup_module(void){
    remove_proc_entry("test_proc", proc_entry);
    printk(KERN_INFO "test_proc module has been cleaned\n");
    vfree(info);
}

ssize_t write_info(struct file *filePointer, const char __user *buff,  
    unsigned long length, void * data){
    if (copy_from_user( &info[write_index], buff, length)) return -2;
    write_index += length;
    info[write_index - 1] = 0;
    return length;
}

// Returns the length
int read_info(char *buff, char **start, off_t offset, int count, 
    int *EOF, void *data){
    int length = 0;
    if (offset > 0) { //  When the end of the file is reached, set the pointer contents so that the read_info sender knows not to overflow the buffer
        *EOF = 1;
        return 0;
    }
    if (read_index >= write_index) read_index = 0;
    length = sprintf(data, "%s\n", &info[read_index]);
    read_index += length;
    return length;
}

