#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include "mbc.h"

#define JOYP_ADDR 0xFF00
#define DIV_ADDR 0xFF04
#define TIMA_ADDR 0xFF05
#define TMA_ADDR 0xFF06
#define TAC_ADDR 0xFF07
#define LCDC_ADDR 0xFF40
#define STAT_ADDR 0xFF41
#define LY_ADDR 0xFF44
#define LYC_ADDR 0xFF45
#define SCY_ADDR 0xFF42
#define SCX_ADDR 0xFF43
#define BGP_ADDR 0xFF47
#define OBP0_ADDR 0xFF48
#define OBP1_ADDR 0xFF49
#define WY_ADDR 0xFF4A
#define WX_ADDR 0xFF4B
#define IE_ADDR 0xFFFF
#define IF_ADDR 0xFF0F

typedef struct Memory {
    uint8_t rom[0x200000]; // 2MB: covers all possible 128 ROM banks for MBC1 (5-bit ROM bank + 2-bit RAM bank in ROM mode)
    uint8_t sram[0x8000];

    uint8_t vram[0x2000];
    uint8_t wram0[0x1000];
    uint8_t wram1[0x1000];
    uint8_t oam[0xA0];
    uint8_t io[0x80];   
    uint8_t hram[0x7F];

    uint8_t joypad_state;

    // I/O registers
    uint8_t joyp;
    uint8_t div;
    uint8_t tima;
    uint8_t tma;
    uint8_t tac;
    uint8_t lcdc;
    uint8_t stat;
    uint8_t ly;
    uint8_t lyc;
    uint8_t scy;
    uint8_t scx;
    uint8_t bgp;
    uint8_t obp0;
    uint8_t obp1;
    uint8_t wy;
    uint8_t wx;
    uint8_t IE;
    uint8_t IF;

    MBC mbc;
} Memory;

void memory_write(Memory* mem, uint16_t addr, uint8_t value);
uint8_t memory_read(Memory* mem, uint16_t addr);

void memory_write16(Memory* mem, uint16_t addr, uint16_t value);
uint16_t memory_read16(Memory* mem, uint16_t addr);

Memory* memory_init();

void set_mbc_type(Memory* mem, MBCType mbcType);

#endif