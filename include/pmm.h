#ifndef PMM_H
#define PMM_H

#include <stdint.h>
#include <stdbool.h>
#include "multiboot.h"

#define PAGE_SIZE 4096

void pmm_init(multiboot_info_t* mbd);
void* pmm_alloc_frame();
void pmm_free_frame(void* physical_address);
uint32_t pmm_get_free_memory();

#endif 
