#include <os/io.h>
#include <os/os.h>
#include <os/types.h>

#define CRT_ADDR_REG 0x3d4
#define CRT_DATA_REG 0x3d5
#define CRT_CURSOR_H 0xe
#define CRT_CURSOR_L 0xf
#define DISPLAY_PTR 0xb8000
#define DISPLAY_WIDTH 80

static int32 print_index = 0;

static void move_cursor() {
	outb(CRT_ADDR_REG, CRT_CURSOR_H);
	outb(CRT_DATA_REG, (print_index >> 8) & 0xff);
	outb(CRT_ADDR_REG, CRT_CURSOR_L);
	outb(CRT_DATA_REG, print_index & 0xff);
}

void kernel_print(const char *string) {
	while (*string) {
		if (*string == '\n') {
			print_index += DISPLAY_WIDTH - print_index % DISPLAY_WIDTH;
			string++;
			continue;
		}
		((char *)DISPLAY_PTR)[print_index++ * 2] = *string;
		string++;
	}
	move_cursor();
}