#include <linux/unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <stdio.h>

#define __NR_proc_stats 340

int main(int argc, char *argv[]){
	int pid;
	printf("Enter a process ID: ");
	scanf("%d", &pid);
	unsigned long mem;
	printf("Enter a memory address: ");
	scanf("%ld", &mem); 
	printf("Process ID: %d\n", pid); 
	printf("Address: %d\n", mem);
	printf("Contacting proc_stats system call...\n\n", pid); 
	syscall(__NR_proc_stats, mem, pid);
	printf("proc_stats memory check complete\n\n");
	return 0;
}
