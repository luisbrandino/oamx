#include "../inc/mbc.h"
#include "../inc/memory.h"

// --- MBC1 and MBC_NONE implementations --- //

void rom_write_mbc_none(Memory* memory, uint16_t addr, uint8_t value)
{
}

uint8_t rom_read_mbc_none(Memory* memory, uint16_t addr)
{
    return memory->rom[addr];    
}

void rom_write_mbc1(Memory* memory, uint16_t addr, uint8_t value)
{
}

uint8_t rom_read_mbc1(Memory* memory, uint16_t addr)
{
    if (addr <= 0x3FFF)
        return rom_read_mbc_none(memory, addr);

    if (addr >= 0x8000)
        return 0xFF;

    uint8_t bank = memory->mbc.rom_bank;

    // bank 0 is fixed, so we set it to bank 1
    if (bank == 0)
        bank = 1;

    // rom_bank has 5 bits, so when banking mode is set to 0 we use the 2 bits of ram_bank register to make it a 7 bit register
    if (memory->mbc.banking_mode == 0)
        bank = (memory->mbc.ram_bank << 5) | bank;

    uint32_t final_addr = 0x4000 * bank + (addr - 0x4000);

    return memory->rom[final_addr];
}