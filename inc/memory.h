#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

#include "mbc.h"

typedef struct {
    uint8_t rom[0x80000]; // 512 KB para suportar jogos grandes, acesso é feito via mbc usando o banco ativo para calcular o offset
    uint8_t sram[0x8000];

    uint8_t vram[0x2000]; // (tela, tiles, background, window)
    uint8_t wram0[0x1000];
    uint8_t wram1[0x1000];
    uint8_t oam[0xA0];
    uint8_t io[0x80];   
    uint8_t hram[0x7F];
    uint8_t ie;

    MBC mbc;
} Memory;

void memory_write(Memory* memory, uint16_t address, uint8_t value);
uint8_t memory_read(Memory* memory, uint16_t address);

void memory_write16(Memory* memory, uint16_t address, uint16_t value);
uint16_t memory_read16(Memory* memory, uint16_t address);

#endif