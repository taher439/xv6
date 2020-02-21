#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"

int main(int argc, char * argv[])
{
  int fd;
  
  if (argc != 3)
  {
    printf(2, "bad usage\n");
    return -1;
  }

  int offset = atoi(argv[2]);

  if((fd = open(argv[1], 0)) < 0){
    printf(2, "testlseek: cannot open %s\n", argv[1]);
    return -1;
  }
  
  int ret = lseek(fd, offset, SEEK_SET);
  printf(0, "ret: %d\n", ret);
  close(fd);
  exit();
}
