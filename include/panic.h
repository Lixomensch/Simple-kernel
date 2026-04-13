#ifndef PANIC_H
#define PANIC_H

#include <stdint.h>
#include "isr.h"

void kernel_panic(const char* message, registers_t* regs);

#endif
