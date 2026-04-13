#ifndef DRIVERS_KEYBOARD_H
#define DRIVERS_KEYBOARD_H

#include <stdint.h>
#include "arch/x86/cpu/isr.h"

#define KEYBOARD_DATA_PORT   0x60
#define KEYBOARD_STATUS_PORT 0x64

void keyboard_handler(registers_t *regs);
void init_keyboard();

#endif
