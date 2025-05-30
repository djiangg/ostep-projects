#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[]) {
  trace("./README");
  int x1 = getcount();
  int rc = fork();

  for (int i = 0; i < 10000; i++)
  {
    int fd = 0;
    if ((fd = open("./README", 0)) < 0)
    {
      printf(1, "cannot open README\n");
      exit();
    }
    close(fd);
  }

  int total = 0;
  if (rc > 0)
  {
    (void) wait();
    int x2 = getcount();
    total += (x2 - x1);
    printf(1, "XV6_TEST_OUTPUT %d\n", total);
  }
  exit();
}
