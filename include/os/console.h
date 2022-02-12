#ifndef OS_CONSOLE_H
#define OS_CONSOLE_H

#include <os/types.h>

void console_init();
void console_clear();
void console_print(const char *data);
void console_printf(const char *fmt, ...);

#endif