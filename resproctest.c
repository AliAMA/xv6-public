
// Tiny executable so that the limit can be filling the proc table.

#include "types.h"
#include "stat.h"
#include "user.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"

#define N  100

/*void
printf(int fd, char *s, ...)
{
  write(fd, s, strlen(s));
}*/

static void
putc(int fd, char c)
{
  write(fd, &c, 1);
}


static void
printint(int fd, int xx, int base, int sgn)
{
  static char digits[] = "0123456789ABCDEF";
  char buf[16];
  int i, neg;
  uint x;

  neg = 0;
  if(sgn && xx < 0){
    neg = 1;
    x = -xx;
  } else {
    x = xx;
  }

  i = 0;
  do{
    buf[i++] = digits[x % base];
  }while((x /= base) != 0);
  if(neg)
    buf[i++] = '-';

  while(--i >= 0)
    putc(fd, buf[i]);
}


void
forktest(void)
{
  //struct proc* x ;
  //memset(&x, 0, sizeof (x));
  int qo = resumeproc();
  printf(1,"\nqo:");
  printint(1,qo,10,0);

  printf(1,"restore proc test OK\n");
}

int
main(void)
{
  forktest();
  exit();
}
