#include "../include/kheap.h"
#include "../include/pmm.h"
#include "../include/paging.h"
#include "../include/kernel.h"

extern uint32_t kernel_end;

typedef struct block_header {
    size_t size;
    bool free;
    struct block_header* next;
} block_header_t;

static block_header_t* heap_start = NULL;
static uint32_t heap_current_top = 0;

void kheap_init() {
    uint32_t start_addr = (uint32_t)(uintptr_t)&kernel_end;
    start_addr = (start_addr & ~0xFFF) + 0x1000; 

    heap_current_top = start_addr + 4096; 

    
    
    
    
    
    
    heap_start = (block_header_t*)(uintptr_t)start_addr;
    heap_start->size = 4096 - sizeof(block_header_t);
    heap_start->free = true;
    heap_start->next = NULL;
}

void* kmalloc(size_t size) {
    if (size == 0) return NULL;
    
    
    size = (size + 3) & ~3;

    block_header_t* current = heap_start;
    block_header_t* best_fit = NULL;

    while (current != NULL) {
        if (current->free && current->size >= size) {
            if (best_fit == NULL || current->size < best_fit->size) {
                best_fit = current;
            }
        }
        current = current->next;
    }

    if (best_fit != NULL) {
        
        if (best_fit->size > size + sizeof(block_header_t) + 4) {
            block_header_t* new_block = (block_header_t*)((uint8_t*)best_fit + sizeof(block_header_t) + size);
            new_block->free = true;
            new_block->size = best_fit->size - size - sizeof(block_header_t);
            new_block->next = best_fit->next;

            best_fit->size = size;
            best_fit->free = false;
            best_fit->next = new_block;
        } else {
            best_fit->free = false;
        }
        return (void*)((uint8_t*)best_fit + sizeof(block_header_t));
    }

    block_header_t* last = heap_start;
    while (last->next != NULL) {
        last = last->next;
    }

    uint32_t pages_needed;
    if (last->free) {
        pages_needed = (size - last->size + 4095) / 4096;
    } else {
        pages_needed = (size + sizeof(block_header_t) + 4095) / 4096;
    }

    uint32_t virt_start = heap_current_top;
    for (uint32_t i = 0; i < pages_needed; i++) {
        void* phys = pmm_alloc_frame();
        if (!phys) {
            kprint("PANIC: KHEAP Out of Physical Memory!\n");
            return NULL;
        }
        vmm_map_page(phys, (void*)(uintptr_t)heap_current_top);
        heap_current_top += 4096;
    }

    if (last->free) {
        last->size += (pages_needed * 4096);
    } else {
        block_header_t* expanded = (block_header_t*)(uintptr_t)virt_start;
        expanded->size = (pages_needed * 4096) - sizeof(block_header_t);
        expanded->free = true;
        expanded->next = NULL;
        last->next = expanded;
    }

    return kmalloc(size);
}

void kfree(void* ptr) {
    if (!ptr) return;
    block_header_t* header = (block_header_t*)((uint8_t*)ptr - sizeof(block_header_t));
    header->free = true;
    
    
    block_header_t* current = heap_start;
    while (current != NULL) {
        if (current->free && current->next && current->next->free) {
            
            if ((uint8_t*)current + sizeof(block_header_t) + current->size == (uint8_t*)current->next) {
                current->size += sizeof(block_header_t) + current->next->size;
                current->next = current->next->next;
            }
        }
        current = current->next;
    }
}
