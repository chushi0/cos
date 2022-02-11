#ifndef OS_OS_H
#define OS_OS_H

#include <os/types.h>

// kernel 入口
extern void kernel_main();

// 打印屏幕信息
extern void kernel_print(const char *string);

// 初始化内存管理
extern bool kernel_memory_init();

#endif // !OS_OS_H
