#ifndef OS_IO_H
#include <os/types.h>

extern uint8 inb(uint16 port);
extern uint16 inw(uint16 port);

extern void outb(uint16 port, uint8 value);
extern void outw(uint16 port, uint16 value);

#endif // !OS_IO_H