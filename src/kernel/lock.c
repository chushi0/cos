#include <os/lock.h>
#include <os/types.h>

void lock(Lock *lock) {
	while (tryLock(lock) != 0)
		;
}

void unlock(Lock *lock) {
	lock->lock_status = 0;
}