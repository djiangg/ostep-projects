#include "types.h"
#include "defs.h"
#include "spinlock.h"

struct {
  struct spinlock lock;
  int enabled;
  int count;
  char pathname[256];
} tracer;

void
tracerinit(void)
{
  tracer.enabled = 0;
  tracer.count = 0;
  for (int i = 0; i < 256; ++i)
    tracer.pathname[i] = 0;
  initlock(&tracer.lock, "tracer");
}

int
get_tracecount(void)
{
  return tracer.count;
}

int
tracer_enabled(void)
{
  return tracer.enabled;
}

void
enable_tracer(const char *pathname)
{
  if (pathname == 0)
    return;

  acquire(&tracer.lock);
  tracer.enabled = 1;
  tracer.count = 0;
  for (int i = 0; i < 256; ++i)
    tracer.pathname[i] = 0;

  char* pathname_ptr = tracer.pathname;
  while ((*pathname_ptr++ = *pathname++) != 0); // strcpy
  release(&tracer.lock);
}

void
update_tracer(const char *pathname)
{
  if (pathname == 0)
    return;

  acquire(&tracer.lock);
  char* pathname_ptr = tracer.pathname;
  while (*pathname && *pathname == *pathname_ptr) // strcmp
    ++pathname, ++pathname_ptr;
  int is_different = (uchar)*pathname - (uchar)*pathname_ptr;

  if (!is_different)
    ++tracer.count;
  release(&tracer.lock);
}
