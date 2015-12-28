#include "types.h"
#include "user.h"
#include "syscall.h"

int
main(int argc, char *argv[])
{
    printf(1, "initial fork count %d\n", SYS_fork);
    if (fork() == 0) {
        printf(1, "child fork count %d\n", SYS_fork);
        printf(1, "child write count %d\n", SYS_write);
    } else {
        wait();
        printf(1, "parent fork count %d\n", SYS_fork);
        printf(1, "parent write count %d\n", SYS_write);
    }
    printf(1, "wait count %d\n", SYS_wait);
    exit();
}
