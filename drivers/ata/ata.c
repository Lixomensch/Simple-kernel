#include "ata.h"
#include "arch/x86/io/io.h"

static int ata_wait_ready() {
    uint8_t status = inb(0x1F7);
    if (status == 0xFF) return 0;

    while ((inb(0x1F7) & 0x80) == 0x80);

    while ((inb(0x1F7) & 0x08) == 0);
    return 1;
}

void ata_read_sector(uint32_t lba, uint8_t *buffer) {
    outb(0x1F6, 0xE0 | ((lba >> 24) & 0x0F));
    outb(0x1F2, 1);
    outb(0x1F3, (uint8_t) lba);
    outb(0x1F4, (uint8_t)(lba >> 8));
    outb(0x1F5, (uint8_t)(lba >> 16));
    outb(0x1F7, 0x20);

    if (!ata_wait_ready()) {
        for(int i=0; i<512; i++) buffer[i] = 0;
        return;
    }

    for (int i = 0; i < 256; i++) {
        uint16_t w = inw(0x1F0);
        buffer[i*2] = (uint8_t)(w & 0xFF);
        buffer[i*2 + 1] = (uint8_t)((w >> 8) & 0xFF);
    }
}
