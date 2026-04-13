#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>
#include <stdbool.h>


#define I86_PDE_PRESENT       1
#define I86_PDE_WRITABLE      2
#define I86_PDE_USER          4
#define I86_PDE_PWT           8
#define I86_PDE_PCD           16
#define I86_PDE_ACCESSED      32
#define I86_PDE_DIRTY         64
#define I86_PDE_4MB           128


#define I86_PTE_PRESENT       1
#define I86_PTE_WRITABLE      2
#define I86_PTE_USER          4
#define I86_PTE_WRITETHROUGH  8
#define I86_PTE_NOT_CACHEABLE 16
#define I86_PTE_ACCESSED      32
#define I86_PTE_DIRTY         64
#define I86_PTE_PAT           128
#define I86_PTE_CPU_GLOBAL    256
#define I86_PTE_LV4_GLOBAL    512

void paging_init();
void vmm_map_page(void* phys, void* virt);

#endif
