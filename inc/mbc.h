#ifndef MBC_H
#define MBC_H

#include <stdint.h>

typedef struct Memory Memory; 

typedef enum {
    MBC_NONE,
    MBC1
} MBCType;

typedef struct MBC {
    MBCType mbc_type;
    uint8_t rom_bank;
    uint8_t ram_bank; 
    uint8_t ram_enabled;
    uint8_t banking_mode;

        void (*rom_write)(Memory* memory, uint16_t addr, uint8_t value);
        uint8_t (*rom_read)(Memory* memory, uint16_t addr);

        void (*ram_write)(Memory* memory, uint16_t addr, uint8_t value);
        uint8_t (*ram_read)(Memory* memory, uint16_t addr);
} MBC;

void rom_write_mbc_none(Memory* memory, uint16_t addr, uint8_t value);
uint8_t rom_read_mbc_none(Memory* memory, uint16_t addr);

void ram_write_mbc_none(Memory* memory, uint16_t addr, uint8_t value);
uint8_t ram_read_mbc_none(Memory* memory, uint16_t addr);

void rom_write_mbc1(Memory* memory, uint16_t addr, uint8_t value);
uint8_t rom_read_mbc1(Memory* memory, uint16_t addr);

void ram_write_mbc1(Memory* memory, uint16_t addr, uint8_t value);
uint8_t ram_read_mbc1(Memory* memory, uint16_t addr);

#endif