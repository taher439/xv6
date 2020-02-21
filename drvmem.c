#include "types.h"
#include "defs.h"
#include "param.h"
#include "traps.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "fs.h"
#include "file.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"

int drvmemread(struct inode *ip, char *buf, int n, uint off)
{
  if (ip->minor == 0)
    return 0;
  
  if (ip->minor == 1)
  {
    memset(buf, 0, n);
    return n;
  }

  if (ip->minor == 2)
  {
    if ((KERNBASE + EXTMEM + off + n) < (KERNBASE + PHYSTOP))
    {
      memmove(buf, (char *) (EXTMEM + KERNBASE + off), n);
      return n;
    }
  }

  return -1;
}

int drvmemwrite(struct inode *ip, char *buf, int n, uint off)
{
  if (ip->minor == 2)
  {
    if ((KERNBASE + EXTMEM + off + n) < (KERNBASE + PHYSTOP))
    {
      memmove((char *) (EXTMEM + KERNBASE + off), buf, n);
      return n;
    }
  }
  return n;
}

void drvmeminit(void) 
{
  devsw[DRVMEM].write = drvmemwrite;
  devsw[DRVMEM].read = drvmemread;
}
