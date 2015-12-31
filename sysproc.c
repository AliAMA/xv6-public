#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "stat.h"
#include "fs.h"
#include "file.h"
#include "fcntl.h"

#define INT_DIGITS 19		/* enough for 64 bit integer */

char *itoa(i)
     int i;
{
  /* Room for INT_DIGITS digits, - and '\0' */
  static char buf[INT_DIGITS + 2];
  char *p = buf + INT_DIGITS + 1;	/* points to terminating '\0' */
  if (i >= 0) {
    do {
      *--p = '0' + (i % 10);
      i /= 10;
    } while (i != 0);
    return p;
  }
  else {			/* i < 0 */
    do {
      *--p = '0' - (i % 10);
      i /= 10;
    } while (i != 0);
    *--p = '-';
  }
  return p;
}

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;
  
  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;
  
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}


int
sys_halt(void)
{
  /*char *p = "Shutdown";
  for( ; *p; p++)
    outb(0x8900, *p);*/
  //char* t = "atohe";
  //exec("/echo", &t);
  //unsigned int adr = 0;
  //adr = (unsigned int)proc;
 
  return 0;
}

//
static struct inode*
create(char *path, short type, short major, short minor)
{
  uint off;
  struct inode *ip, *dp;
  char name[DIRSIZ];

  if((dp = nameiparent(path, name)) == 0)
    return 0;
  ilock(dp);

  if((ip = dirlookup(dp, name, &off)) != 0){
    iunlockput(dp);
    ilock(ip);
    if(type == T_FILE && ip->type == T_FILE)
      return ip;
    iunlockput(ip);
    return 0;
  }

  if((ip = ialloc(dp->dev, type)) == 0)
    panic("create: ialloc");

  ilock(ip);
  ip->major = major;
  ip->minor = minor;
  ip->nlink = 1;
  iupdate(ip);

  if(type == T_DIR){  // Create . and .. entries.
    dp->nlink++;  // for ".."
    iupdate(dp);
    // No ip->nlink++ for ".": avoid cyclic ref count.
    if(dirlink(ip, ".", ip->inum) < 0 || dirlink(ip, "..", dp->inum) < 0)
      panic("create dots");
  }

  if(dirlink(dp, name, ip->inum) < 0)
    panic("create: dirlink");

  iunlockput(dp);

  return ip;
}

static int
fdalloc(struct file *f)
{
  int fd;

  for(fd = 0; fd < NOFILE; fd++){
    if(proc->ofile[fd] == 0){
      proc->ofile[fd] = f;
      return fd;
    }
  }
  return -1;
}


struct file* openfile(char* ipath, int iomode,int* inf)
{
  char *path;
  int fd, omode;
  struct file *f;
  struct inode *ip;

  path = ipath;
  omode = iomode;

  begin_op();

  if(omode & O_CREATE){
    ip = create(path, T_FILE, 0, 0);
    if(ip == 0){
      end_op();
      return 0;
    }
  } else {
    if((ip = namei(path)) == 0){
      end_op();
      return 0;
    }
    ilock(ip);
    if(ip->type == T_DIR && omode != O_RDONLY){
      iunlockput(ip);
      end_op();
      return 0;
    }
  }

  if((f = filealloc()) == 0 || (fd = fdalloc(f)) < 0){
    if(f)
      fileclose(f);
    iunlockput(ip);
    end_op();
    return 0;
  }
  iunlock(ip);
  end_op();

  f->type = FD_INODE;
  f->ip = ip;
  f->off = 0;
  f->readable = !(omode & O_WRONLY);
  f->writable = (omode & O_WRONLY) || (omode & O_RDWR);
  *inf = fd;
  return f;
}

int writefile(struct file* f,char* p, int n){
  return filewrite(f, p, n);
}

int closefile(int ifd, struct file * inf){
  int fd = ifd;
  struct file *f;

  f = inf;
  proc->ofile[fd] = 0;
  fileclose(f);
  return 0;
}

int
sys_getproc(void)
{
  //struct proc* this_proc = proc;
  struct proc** out_proc = 0;
  struct file* myfile = 0 ;
  if (argptr(0, (void*)&out_proc,sizeof(out_proc)) < 0)
    return -1;
  *out_proc = proc;
  //filewrite(struct file *f, char *addr, int n)
  int fd=0;
  char* addr = "/kt1.txt";
  char* chr = "";
  
  myfile = openfile(addr, O_CREATE, &fd);
  closefile(fd,myfile);
  
  myfile = openfile(addr, O_RDWR, &fd);
  chr = (*out_proc)->name;
  writefile(myfile, chr, 16);
  chr = "\n";
  writefile(myfile, chr, 1);
  chr = itoa((int)((*out_proc)->sz));
  writefile(myfile, chr, sizeof(chr));
  chr = "\n";
  writefile(myfile, chr, 1);
  chr = (*out_proc)->kstack;
  writefile(myfile, chr, sizeof((*out_proc)->kstack));
  chr = "\n";
  writefile(myfile, chr, 1);
  enum procstate this_procstate = (*out_proc)->state;
  if (this_procstate == UNUSED)
    chr = "0";
  else if (this_procstate == EMBRYO)
    chr = "1";
  else if (this_procstate == SLEEPING)
    chr = "2";
  else if (this_procstate == RUNNABLE)
    chr = "3";
  else if (this_procstate == RUNNING)
    chr = "4";
  else if (this_procstate == ZOMBIE)
    chr = "5";
  writefile(myfile, chr, 1);
  chr = "\n";
  writefile(myfile, chr, 1);
  chr = itoa((int)((*out_proc)->context->edi));
  writefile(myfile, chr, sizeof(chr));
  chr = "\n";
  writefile(myfile, chr, 1);
  chr = itoa((int)((*out_proc)->context->esi));
  writefile(myfile, chr, sizeof(chr));
  chr = "\n";
  writefile(myfile, chr, 1);
  chr = itoa((int)((*out_proc)->context->ebx));
  writefile(myfile, chr, sizeof(chr));
  chr = "\n";
  writefile(myfile, chr, 1);
  chr = itoa((int)((*out_proc)->context->ebp));
  writefile(myfile, chr, sizeof(chr));
  chr = "\n";
  writefile(myfile, chr, 1);
  chr = itoa((int)((*out_proc)->context->eip));
  writefile(myfile, chr, sizeof(chr));

  closefile(fd,myfile);
  //filewrite(myf, addr,2);
  //fileclose(myf);

  //sys_write(1,this_proc->kstack, sizeof(this_proc->kstack));
  return (*out_proc)->pid;
  //return this_proc;
}
