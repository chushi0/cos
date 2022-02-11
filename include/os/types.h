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

#define _packed __attribute__((packed))

#define _debugger __asm("xchg %bx, %bx")

#endif // !COS_TYPES_H
