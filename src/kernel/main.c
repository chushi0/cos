#include <os/console.h>
#include <os/os.h>
#include <os/types.h>

static void print_memory_info() {
	_size total, usable, free;
	memory_info(&total, &usable, &free);
	console_printf(" - Total: %dK\n - Usable: %dK\n - Free: %dK\n", total >> 10,
				   usable >> 10, free >> 10);
}

void kernel_main() {
	bool res;
	console_init();
	console_print("Hello COS!\nKernel loaded, initializing...\n");
	res = memory_init();
	if (!res) {
		console_print("Detect Memory Error!\n");
		return;
	}
	console_print("Detect Memory Success:\n");
	print_memory_info();

	return;
}