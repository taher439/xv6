#include "sem.h"
#include "types.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"
#include "fs.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "file.h"

struct {
  struct spinlock lock;
  struct sem sems[NSEM];
} semtable;

struct sem* sem_alloc(int cnt)
{
  struct sem *s;
  aquire(&semtable.lock);
  for (s = semtable.sems; s < semtable.sems + NSEM; s++)
    if (s == 0) {
       s = (struct sem*) malloc(sizeof(struct sem));
       s->cnt = cnt;
       s->last = 0;
       release(&semtable.lock);
       return s;
    }
   release(&semtable.lock);
   return 0;
}

int sem_create(int cnt, sum) 
{
  struct sem * s;
  struct file * f;
  int fd;
  struct proc * curproc = myproc();

  if ((f = filealloc()) == 0 || (s = sem_alloc(cnt)) == 0)
    return -1;
  
  f->sm = s;
  f->type = FD_SEM;
  for (fd = 0; fd < NOFILE; fd++)
    if (curproc->ofile[fd] == 0) {
      curproc->ofile[fd] = f;
      return fd;
    }

  return -1;
}

int sem_p(int fd) 
{
  struct file * f;
  struct proc * curproc = myproc();
  
  if (fd < 0 || fd >= NOFILE || (f = curproc->ofile[fd]) == 0)
    return -1;
  
  acquire(&f->sm->lock);
  while (f->sm->cnt == 0) 
  {
    f->sm->procs[f->sm->last] = curproc;
    f->sm->last = (f->sm->last + 1) % NPROCS;
    sleep(curproc, &f->sm->lock);
  }
  f->sm->cnt--;
  release(&f->sm->lock);
}

int sem_v(int fd) 
{
  struct file * f;
  struct proc * curproc = myproc();
  
  if (fd < 0 || fd >= NOFILE || (f = curproc->ofile[fd]) == 0)
    return -1;
  
  struct proc * lastproc = f->sm->procs[f->sm->last];
  acquire(&f->sm->lock);
  
  f->sm->cnt++;
  if (f->sm->cnt == 0 && lastproc != 0) 
  {
    wakeup(lastproc);
    f->sm->procs[f->sm->last] = 0;
    f->sm->last = (f->sm->last + 1) % NPROCS;
  }
  release(&f->sm->lock);
  return 0;
}

void seminit(void) {
  initlock(&semtable.lock, "semtable");
}
