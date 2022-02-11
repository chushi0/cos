#include <os/mem.h>
#include <os/os.h>
#include <os/types.h>

#define KERNEL_CODE_START 0x10000
#define KERNEL_CODE_END 0x29000

typedef struct {
	int64 BaseAddr;
	int64 Length;
	int32 Type;
} MemDesc;

static struct FMN;
static struct FMN { struct FMN *next; };
typedef struct FMN FreeMemNode;

// 内存检测数据
static MemDesc *memory_desc;
// 内存检测数据大小
static uint16 memory_desc_count;
// 空闲内存链表
static FreeMemNode *free_memory_linkedlist;
// 空闲内存链表长度
static uint32 free_memory_linkedlist_size;

static void link_free_memory() {
	free_memory_linkedlist = NULL;
	free_memory_linkedlist_size = 0;
	for (uint16 i = 0; i < memory_desc_count; i++) {
		if (memory_desc[i].Type != 1) {
			continue;
		}
		uint64 addr = memory_desc[i].BaseAddr;
		uint64 length = memory_desc[i].Length >> 10;
		while (length-- > 0) {
			uint64 memAddr = addr + (length << 10);
			if (memAddr == memory_desc || memAddr < KERNEL_CODE_END) {
				continue;
			}
			FreeMemNode *node = (FreeMemNode *)memAddr;
			node->next = free_memory_linkedlist;
			free_memory_linkedlist = node;
			free_memory_linkedlist_size++;
		}
	}
}

bool kernel_memory_init() {
	// loader 进行的内存检测数据
	char *ptr = *(char **)0x1010;
	uint16 count = *(uint16 *)ptr;
	MemDesc *mems = (MemDesc *)(ptr + 2);
	// 内存检测数据占用空间
	uint32 size = count * sizeof(MemDesc);
	// 寻找第一段可用且空间足够的内存
	// 之前bootloader都加载了，这里应该不会覆盖内核代码
	bool found = false;
	for (uint16 i = 0; i < count; i++) {
		if (mems[i].Type == 1 && mems[i].Length >= size) {
			uint64 base_addr = mems[i].BaseAddr;
			// 避免覆盖中断向量表
			if (base_addr == 0) {
				if (mems[i].Length - 0x400 < size) {
					continue;
				}
				base_addr = 0x400;
			}
			// 将内存检测数据复制到该内存
			memcpy(base_addr, mems, size);
			memory_desc = base_addr;
			memory_desc_count = count;
			// 管理空闲内存
			link_free_memory();
			found = true;
			break;
		}
	}
	return found;
}

void *malloc() {
	if (free_memory_linkedlist_size == 0) {
		return NULL;
	}
	free_memory_linkedlist_size--;
	void *memory = free_memory_linkedlist;
	free_memory_linkedlist = free_memory_linkedlist->next;
	return memory;
}

void free(void *ptr) {
	FreeMemNode *node = ptr;
	node->next = free_memory_linkedlist;
	free_memory_linkedlist_size++;
}

void memcpy(void *dst, void *src, uint32 size) {
	char *cdst = dst;
	char *csrc = src;
	if (dst > src) {
		cdst += size;
		csrc += size;
		while (size > 0) {
			*cdst = *csrc;
			size--;
			csrc--;
			cdst--;
		}
	} else {
		while (size > 0) {
			*cdst = *csrc;
			size--;
			csrc++;
			cdst++;
		}
	}
}