#include "types.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  char* null = 0;
  printf(1, "XV6_TEST_OUTPUT %d\n", trace(null));
  exit();
}
