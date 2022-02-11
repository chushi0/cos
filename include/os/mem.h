#ifndef OS_MEM_H
#define OS_MEM_H

#include <os/types.h>

// 申请固定大小的内存（1K）
void *malloc();
// 释放内存
void free(void *ptr);

void memcpy(void *dst, void *src, uint32 size);

#endif // !OS_MEM_H
