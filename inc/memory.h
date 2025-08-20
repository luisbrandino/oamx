#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

#include "mbc.h"

typedef struct {
    MBC mbc;
} Memory;

void memory_write(Memory* memory, uint16_t address, uint8_t value);
uint8_t memory_read(Memory* memory, uint16_t address);

void memory_write16(Memory* memory, uint16_t address, uint16_t value);
uint16_t memory_read16(Memory* memory, uint16_t address);

#endif