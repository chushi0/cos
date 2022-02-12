#ifndef COS_TYPES_H
#define COS_TYPES_H

#define EOF -1
#define NULL ((void *)0)
#define EOS '\0'

#define bool _Bool
#define true 1
#define false 0

typedef signed char int8;
typedef signed short int16;
typedef signed int int32;
typedef signed long long int64;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

typedef uint32 _size;

#define _packed __attribute__((packed))

#define _debugger __asm("xchg %bx, %bx")

typedef char *va_list;
#define va_start(ap, v) (ap = (va_list)&v + sizeof(char *))
#define va_arg(ap, t) (*(t *)((ap += sizeof(char *)) - sizeof(char *)))
#define va_end(ap) (ap = (va_list)0)

#endif // !COS_TYPES_H
