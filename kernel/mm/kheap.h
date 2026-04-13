#ifndef KERNEL_MM_KHEAP_H
#define KERNEL_MM_KHEAP_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

void kheap_init();
void* kmalloc(size_t size);
void kfree(void* ptr);

#endif
