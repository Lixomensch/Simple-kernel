#include "pmm.h"
#include "kernel/printk.h"

#define BLOCKS_PER_BYTE 8
#define BLOCKS_COUNT 1048576
#define BITMAP_SIZE (BLOCKS_COUNT / BLOCKS_PER_BYTE)

static uint8_t memory_bitmap[BITMAP_SIZE] = {0};

static uint32_t used_frames = 0;
static uint32_t total_frames = 0;

extern uint32_t kernel_start;
extern uint32_t kernel_end;

static inline void bitmap_set(uint32_t bit) {
    memory_bitmap[bit / 8] |= (1 << (bit % 8));
}

static inline void bitmap_clear(uint32_t bit) {
    memory_bitmap[bit / 8] &= ~(1 << (bit % 8));
}

static inline bool bitmap_test(uint32_t bit) {
    return memory_bitmap[bit / 8] & (1 << (bit % 8));
}

void pmm_init(multiboot_info_t* mbd) {

    for (uint32_t i = 0; i < BITMAP_SIZE; i++) {
        memory_bitmap[i] = 0xFF;
    }

    if (!(mbd->flags & (1 << 6))) {
        kprint("ERROR: Multiboot memory map not provided!\n");
        return;
    }

    multiboot_memory_map_t* mmap = (multiboot_memory_map_t*) mbd->mmap_addr;
    while ((uint32_t)mmap < mbd->mmap_addr + mbd->mmap_length) {
        if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE) {
            uint32_t block_addr = mmap->addr_low;
            uint32_t block_len = mmap->len_low;

            for (uint32_t i = 0; i < block_len; i += PAGE_SIZE) {
                bitmap_clear((block_addr + i) / PAGE_SIZE);
                total_frames++;
            }
        }
        mmap = (multiboot_memory_map_t*) ((uint32_t)mmap + mmap->size + sizeof(mmap->size));
    }

    uint32_t k_start = (uint32_t)&kernel_start;
    uint32_t k_end = (uint32_t)&kernel_end;
    k_start = (k_start / PAGE_SIZE) * PAGE_SIZE;

    for (uint32_t i = k_start; i <= k_end; i += PAGE_SIZE) {
        bitmap_set(i / PAGE_SIZE);
    }

    bitmap_set(0);

    bitmap_set(0xB8000 / PAGE_SIZE);
}

void* pmm_alloc_frame() {
    for (uint32_t i = 0; i < BITMAP_SIZE * 8; i++) {
        if (!bitmap_test(i)) {
            bitmap_set(i);
            used_frames++;
            return (void*)(i * PAGE_SIZE);
        }
    }
    return 0;
}

void pmm_free_frame(void* physical_address) {
    uint32_t addr = (uint32_t)physical_address;
    uint32_t frame = addr / PAGE_SIZE;
    if (bitmap_test(frame)) {
        bitmap_clear(frame);
        used_frames--;
    }
}

uint32_t pmm_get_free_memory() {
    return (total_frames - used_frames) * PAGE_SIZE;
}
