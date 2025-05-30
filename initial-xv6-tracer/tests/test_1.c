#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  int fd = 0;
  if ((fd = open("./README", 0)) < 0)
  {
    printf(1, "cannot open README\n");
    exit();
  }
  close(fd);
  int x1 = getcount();

  trace("./README");
  for (int i = 0; i < 5; i++)
  {
    if ((fd = open("./README", 0)) < 0)
    {
      printf(1, "cannot open README\n");
      exit();
    }
    close(fd);
    if ((fd = open("./forktest", 0)) < 0)
    {
      printf(1, "cannot open forktest\n");
      exit();
    }
    close(fd);
  }
  int x2 = getcount();

  trace("./forktest");
  int x3 = getcount();
  for (int i = 0; i < 1000; i++)
  {
    if((fd = open("./forktest", 0)) < 0)
    {
      printf(1, "cannot open forktest\n");
      exit();
    }
    close(fd);
  }
  int x4 = getcount();

  printf(1, "XV6_TEST_OUTPUT %d %d %d %d\n", x1, x2, x3, x4);
  exit();
}
