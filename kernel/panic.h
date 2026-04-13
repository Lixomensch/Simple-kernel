#ifndef KERNEL_PANIC_H
#define KERNEL_PANIC_H

#include <stdint.h>
#include "arch/x86/cpu/isr.h"

void kernel_panic(const char* message, registers_t* regs);

#endif
