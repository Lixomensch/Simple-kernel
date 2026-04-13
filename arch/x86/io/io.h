#ifndef ARCH_X86_IO_H
#define ARCH_X86_IO_H

#include <stdint.h>

void outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);

#endif
