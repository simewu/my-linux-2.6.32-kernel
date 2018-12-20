#include <linux/unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <stdio.h>

#define __NR_virtual_address_space 341

int main(int argc, char *argv[]){
	int pid;
	printf("Enter a process ID: ");
	scanf("%d", &pid);
	printf("Process ID %d\n", pid);
	printf("Contacting virtual_address_space system call...\n\n");
	syscall(__NR_virtual_address_space, pid); 
	printf("virtual_address_space test complete\n\n");
	return 0;
}
