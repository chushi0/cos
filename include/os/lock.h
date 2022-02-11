#ifndef OS_LOCK_H
#define OS_LOCK_H

#include <os/mem.h>
#include <os/types.h>

typedef struct {
	uint8 lock_status;
} Lock;

Lock *mallocLock();
void freeLock(Lock *plock);

bool tryLock(Lock *lock);
void lock(Lock *lock);
void unlock(Lock *lock);

#endif