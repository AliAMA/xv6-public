
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

void
printf(int fd, char *s, ...)
{
  write(fd, s, strlen(s));
}

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
  int /*n, pid,*/ rpid=0;
  rpid = getpid();
  struct proc* x ;
  memset(&x, 0, sizeof (x));
  int qo = getproc(&x);
  char c = 0;
  read(1,&c,1);
  
  write(1,&c,1);
  //int z = x->pid;
  //char* y = x.kstack;
  //char* x;

  
  
  printf(1,"fork test, pid: ");
  printint(1,rpid,10,0);
  printf(1,"-----\n");
  //printint(1,x->context->edi,10,0);
  printf(1," - - -");
  //printint(1,x->pid,10,0);
  //printf(1,"%s\n",x->name);
  printf(1,"\nqo:");
  //printint(1,halt(),10,0);
  printint(1,qo,10,0);

  /*for(n=0; n<N; n++){
    pid = fork();
    if(pid < 0)
      break;
    if(pid == 0)
      exit();
  }
  
  if(n == N){
    printf(1,"fork claimed to work %d times!\n", N);
    exit();
  }
  
  for(; n > 0; n--){
    if(wait() < 0){
      printf(1,"wait stopped early\n");
      exit();
    }
  }
  
  if(wait() != -1){
    printf(1,"wait got too many\n");
    exit();
  }
  */
  printf(1,"fork test OK\n");
}

int
main(void)
{
  forktest();
  exit();
}
