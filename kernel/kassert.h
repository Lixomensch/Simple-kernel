#ifndef KERNEL_KASSERT_H
#define KERNEL_KASSERT_H

#include "kernel/printk.h"

#define KASSERT(condition)                                          \
    do {                                                            \
        if (!(condition)) {                                         \
            kprint("KASSERT FAILED: " #condition "\n");             \
            kprint("  File: " __FILE__ "\n");                       \
            while (1) { __asm__ volatile("cli; hlt"); }             \
        }                                                           \
    } while (0)

#endif
