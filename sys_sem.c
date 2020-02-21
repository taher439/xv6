#include "types.h"
#include "defs.h"
#include "param.h"
#include "stat.h"
#include "mmu.h"
#include "proc.h"
#include "fs.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "file.h"
#include "fcntl.h"
#include "sem.h"

int sys_sem_create(void) 
{
  int cnt;
  if (argint(0, &cnt) < 0)
    return -1;
  return sem_create(cnt);
}

int sys_sem_p(void)
{
  int fd;
  if (argint(0, &cnt) < 0)
    return -1;
  return sem_p(fd);
}

int sys_sem_v(void)
{
  int fd;
  if (argint(0, &cnt) < 0)
    return -1;
  return sem_v(fd);
}
