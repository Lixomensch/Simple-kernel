#include "idt.h"

__attribute__((aligned(0x10)))
static struct idt_entry idt[IDT_ENTRIES];
static struct idtr idtr;

extern void idt_load(void);

void idt_set_descriptor(uint8_t vector, void *isr, uint8_t flags) {
    uint32_t isr_addr = (uint32_t)(uintptr_t)isr;
    idt[vector].isr_low = (uint16_t)(isr_addr & 0xFFFF);
    idt[vector].kernel_cs = 0x08;
    idt[vector].reserved = 0;
    idt[vector].attributes = flags;
    idt[vector].isr_high = (uint16_t)((isr_addr >> 16) & 0xFFFF);
}

extern void* isr_stub_table[];

void idt_init() {
    idtr.base = (uint32_t)(uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(struct idt_entry) * IDT_ENTRIES - 1;

    for (uint8_t vector = 0; vector < 32; vector++) {
        idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
    }

    for (uint8_t vector = 32; vector < 48; vector++) {
        idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
    }

    __asm__ volatile ("lidt %0" : : "m"(idtr));
    __asm__ volatile ("sti");
}
