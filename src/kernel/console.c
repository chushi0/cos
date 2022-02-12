#include <os/console.h>
#include <os/io.h>
#include <os/mem.h>
#include <os/string.h>
#include <os/types.h>

#define CRT_ADDR_REG 0x3d4
#define CRT_DATA_REG 0x3d5
#define CRT_START_ADDR_H 0xC
#define CRT_START_ADDR_L 0xD
#define CRT_CURSOR_H 0xe
#define CRT_CURSOR_L 0xf
#define CONSOLE_WIDTH 80
#define CONSOLE_HEIGHT 25
#define CONSOLE_SIZE (CONSOLE_WIDTH * CONSOLE_HEIGHT)
#define CONSOLE_BYTES (CONSOLE_SIZE * 2)
#define MEMORY_START 0xb8000
#define MEMORY_LENGTH 0x4000
#define MEMORY_END (MEMORY_START + MEMORY_LENGTH)
#define LINEDATA (CONSOLE_WIDTH * 2)

#define WHITE_SPACE 0x0720

static uint16 cursor_position;
static uint16 memory_offset;
static uint16 style;

static void set_cursor() {
	outb(CRT_ADDR_REG, CRT_CURSOR_H);
	outb(CRT_DATA_REG, (cursor_position >> 8) & 0xff);
	outb(CRT_ADDR_REG, CRT_CURSOR_L);
	outb(CRT_DATA_REG, cursor_position & 0xff);
}

static void set_memory() {
	outb(CRT_ADDR_REG, CRT_START_ADDR_H);
	outb(CRT_DATA_REG, (memory_offset >> 8) & 0xff);
	outb(CRT_ADDR_REG, CRT_START_ADDR_L);
	outb(CRT_DATA_REG, memory_offset & 0xff);
}

void console_init() {
	cursor_position = 0;
	memory_offset = 0;
	style = 0x0700;
	set_cursor();
	set_memory();
	console_clear();
}

static void clear_range(uint16 *start, uint16 *end) {
	do {
		*start = WHITE_SPACE;
		start++;
	} while (start != end);
}

void console_clear() {
	clear_range((uint16 *)MEMORY_START, (uint16 *)MEMORY_END);
}

static void checkCRLF() {
	if (cursor_position >= CONSOLE_SIZE) {
		memcpy(MEMORY_START, MEMORY_START + LINEDATA, CONSOLE_BYTES - LINEDATA);
		cursor_position -= CONSOLE_WIDTH;
		clear_range((uint16 *)(MEMORY_START + (cursor_position << 1)),
					(uint16 *)(MEMORY_START + CONSOLE_BYTES + 2));
	}
}

static void put(char ch) {
	_size pos = cursor_position << 1;
	uint16 b = style | ch;
	*(uint16 *)(MEMORY_START + pos) = b;
	cursor_position++;
	checkCRLF();
}

static void crlf() {
	uint16 line = cursor_position / CONSOLE_WIDTH;
	line++;
	cursor_position = line * CONSOLE_WIDTH;
	checkCRLF();
}

static void tab() {
	uint16 rep = 4 - cursor_position % 4;
	if (rep == 0) {
		rep = 4;
	}
	for (int i = 0; i < rep; i++) {
		put(' ');
	}
}

static void back() {
	if (cursor_position % CONSOLE_WIDTH > 0) {
		cursor_position--;
		_size pos = cursor_position << 1;
		*(uint16 *)(MEMORY_START + pos) = WHITE_SPACE;
	}
}

static void print_char(const char ch) {
	switch (ch) {
		case '\r':
			break;
		case '\n':
			crlf();
			break;
		case '\t':
			tab();
			break;
		case '\b':
			back();
			break;
		case '\f':
			console_clear();
			break;
		default:
			put(ch);
			break;
	}
}

void console_print(const char *data) {
	while (*data != 0) {
		print_char(*data);
		data++;
	}
	set_cursor();
}

static void print_uint32(uint32 data) {
	if (data == 0) {
		console_print("0");
		return;
	}
	char buf[11];
	buf[10] = 0;
	_size i = 10;
	while (data > 0) {
		i--;
		buf[i] = data % 10 + '0';
		data /= 10;
	}
	console_print(buf + i);
}

static void print_int32(int32 data) {
	if (data == 0x80000000) {
		console_print("-2147483648");
		return;
	}
	if (data < 0) {
		print_char('-');
		print_uint32(-data);
		return;
	}
	print_uint32(data);
}

void console_printf(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	while (*fmt != 0) {
		if (*fmt != '%') {
			print_char(*fmt);
			fmt++;
			continue;
		}
		fmt++;
		if (string_starts(fmt, "d")) {
			fmt += 1;
			print_int32(va_arg(ap, int32));
		} else if (string_starts(fmt, "ud")) {
			fmt += 2;
			print_uint32(va_arg(ap, uint32));
		} else if (string_starts(fmt, "s")) {
			fmt += 1;
			console_print(va_arg(ap, const char *));
		} else if (string_starts(fmt, "n")) {
			fmt += 1;
			crlf();
		}
	}
	set_cursor();
	va_end(ap);
}
