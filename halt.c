#include "types.h" 
#include "defs.h"
#include "param.h"
#include "traps.h"
#include "spinlock.h"
#include "fs.h"
#include "file.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"

int
halt(){
  char *p = "Shutdown";
  for( ; *p; p++)
    outb(0x8900, *p);
  return 0;
}
