#include "paging.h"
#include "pmm.h"
#include "kernel/printk.h"

__attribute__((aligned(4096)))
static uint32_t page_directory[1024];

__attribute__((aligned(4096)))
static uint32_t first_page_table[1024];

extern void load_page_directory(uint32_t*);
extern void enable_paging();

void paging_init() {

    for (int i = 0; i < 1024; i++) {
        page_directory[i] = 0x00000002;
    }

    for (int i = 0; i < 1024; i++) {

        first_page_table[i] = (i * 4096) | I86_PTE_PRESENT | I86_PTE_WRITABLE;
    }

    page_directory[0] = ((uint32_t)(uintptr_t)first_page_table) | I86_PDE_PRESENT | I86_PDE_WRITABLE;

    __asm__ volatile("mov %0, %%cr3":: "r"(page_directory));
    uint32_t cr0;
    __asm__ volatile("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000;
    __asm__ volatile("mov %0, %%cr0":: "r"(cr0));
}

void vmm_map_page(void* phys, void* virt) {

    uint32_t p_addr = (uint32_t)(uintptr_t)phys & ~0xFFF;
    uint32_t v_addr = (uint32_t)(uintptr_t)virt & ~0xFFF;

    uint32_t pd_index = v_addr >> 22;
    uint32_t pt_index = (v_addr >> 12) & 0x03FF;

    uint32_t* pd = page_directory;
    uint32_t pde = pd[pd_index];

    uint32_t* pt;
    if ((pde & I86_PDE_PRESENT)) {
        pt = (uint32_t*)(uintptr_t)(pde & ~0xFFF);
    } else {

        pt = (uint32_t*)pmm_alloc_frame();
        if (!pt) {
            kprint("PANIC: Out of memory mapping new page context!\n");
            return;
        }

        for (int i = 0; i < 1024; i++) {
            pt[i] = 0;
        }

        pd[pd_index] = ((uint32_t)(uintptr_t)pt) | I86_PDE_PRESENT | I86_PDE_WRITABLE;
    }

    pt[pt_index] = p_addr | I86_PTE_PRESENT | I86_PTE_WRITABLE;

    __asm__ volatile("invlpg (%0)" :: "r"(v_addr) : "memory");
}
