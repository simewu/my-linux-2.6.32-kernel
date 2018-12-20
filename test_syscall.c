#include <linux/unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <stdio.h>

#define __NR_helloworld 337
#define __NR_printother 338
#define __NR_printself 339

int main(int argc, char *argv[])
{
    syscall(__NR_helloworld);
    syscall(__NR_printself);
    syscall(__NR_printother, 2331);
    return 0;
}

