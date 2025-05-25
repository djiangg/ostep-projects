#include "types.h"
#include "defs.h"
#include "spinlock.h"

struct {
    struct spinlock lock;
    int count;
} readcounter;

void
readcountinit(void)
{
    readcounter.count = 0;
    initlock(&readcounter.lock, "readcount");
}

void
increase_readcount(void)
{
    acquire(&readcounter.lock);
    ++readcounter.count;
    release(&readcounter.lock);
}

int
get_readcount(void)
{
    return readcounter.count;
}
