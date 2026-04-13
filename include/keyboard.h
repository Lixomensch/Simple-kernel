#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>
#include "isr.h"

void keyboard_handler(registers_t *regs);
void init_keyboard();

#endif
