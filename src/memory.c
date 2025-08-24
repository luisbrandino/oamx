#include "../inc/memory.h"
#include <stdlib.h>
#include <string.h>

void memory_write(Memory* mem, uint16_t addr, uint8_t value)
{
    if (addr <= 0x7FFF)
    {
        mem->mbc.rom_write(mem, addr, value);
    }
    else if (addr >= 0xA000 && addr <= 0xBFFF)
    {
        mem->mbc.ram_write(mem, addr, value);
    }
}

uint8_t memory_read(Memory* mem, uint16_t addr)
{
    if (addr <= 0x7FFF)
    {
        return mem->mbc.rom_read(mem, addr);
    }
    else if (addr >= 0xA000 && addr <= 0xBFFF)
    {
        return mem->mbc.ram_read(mem, addr);
    }

    return 0xFF;
}

Memory* memory_init()
{
    Memory *mem = (Memory*) malloc(sizeof(Memory));
    memset(mem, 0, sizeof(Memory));

    // for the sake of simplicity
    mem->mbc.mbc_type = MBC1;
    mem->mbc.rom_write = rom_write_mbc1;
    mem->mbc.rom_read = rom_read_mbc1;
    mem->mbc.ram_write = ram_write_mbc1;
    mem->mbc.ram_read = ram_read_mbc1;

    return mem;
}