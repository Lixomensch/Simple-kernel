#ifndef DRIVERS_ATA_H
#define DRIVERS_ATA_H

#include <stdint.h>

void ata_read_sector(uint32_t lba, uint8_t *buffer);

#endif
