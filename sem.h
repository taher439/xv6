#define NSEM 30
#define NPROCS 128

struct sem {
  int cnt;
  struct spinlock lock;
  struct proc * procs[NPROCS];
  int last;
};

