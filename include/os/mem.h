#ifndef OS_MEM_H
#define OS_MEM_H

#include <os/types.h>

// 初始化内存管理
bool memory_init();
// 内存信息
// 总量 / 可用 / 空闲
void memory_info(_size *total, _size *usable, _size *free);

// 申请固定大小的内存（1K）
void *malloc();
// 释放内存
void free(void *ptr);

void memcpy(char *dst, char *src, _size size);
void memset(char *dst, char ch, _size size);

#endif // !OS_MEM_H
