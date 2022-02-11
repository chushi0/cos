#include <os/lock.h>
#include <os/os.h>
#include <os/types.h>

void kernel_main() {
	bool res;
	kernel_print("Hello COS!\nKernel loaded, initializing...\n");
	res = kernel_memory_init();
	if (!res) {
		kernel_print("Memory Initialize Error!\n");
		return;
	}

	Lock l = {0};
	res = tryLock(&l);
	if (res == 0) {
		kernel_print("Enter lock\n");
	} else {
		kernel_print("no enter Lock\n");
	}
	res = tryLock(&l);
	if (res == 0) {
		kernel_print("Enter lock\n");
	} else {
		kernel_print("no enter Lock\n");
	}
	unlock(&l);
	res = tryLock(&l);
	if (res == 0) {
		kernel_print("Enter lock\n");
	} else {
		kernel_print("no enter Lock\n");
	}
	unlock(&l);
	lock(&l);
	kernel_print("Enter lock\n");
	lock(&l);
	kernel_print("Enter lock\n");

	return;
}