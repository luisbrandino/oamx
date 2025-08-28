#include "../inc/memory.h"
#include <stdlib.h>
#include <string.h>

void memory_write(Memory* mem, uint16_t addr, uint8_t value)
{
    if (addr <= 0x7FFF)
    {
        mem->mbc.rom_write(mem, addr, value);
    }
    else if (addr <= 0x9FFF)
    {
        mem->vram[addr - 0x8000] = value;
    }
    else if (addr <= 0xBFFF)
    {
        mem->mbc.ram_write(mem, addr, value);
    }
    else if (addr <= 0xCFFF)
    {
        mem->wram0[addr - 0xC000] = value;
    }
    else if (addr <= 0xDFFF)
    {
        mem->wram1[addr - 0xD000] = value;
    }
    else if (addr <= 0xFDFF)
    {
        uint16_t mirrored = addr - 0x2000;

        if (mirrored > 0xCFFF)
        {
            mem->wram1[mirrored - 0xD000] = value;
            return;
        }

        mem->wram0[mirrored - 0xC000] = value;
    }
    else if (addr <= 0xFE9F)
    {
        mem->oam[addr - 0xFE00] = value;
    }
    else if (addr <= 0xFEFF)
    {
        return;
    }
    else if (addr <= 0xFF7F)
    {
        mem->io[addr - 0xFF00] = value;
    }
    else if (addr <= 0xFFFE)
    {
        mem->hram[addr - 0xFF80] = value;
    }
    else if (addr == 0xFFFF)
    {
        mem->ie = value;
    }
}

uint8_t memory_read(Memory* mem, uint16_t addr)
{
    if (addr <= 0x7FFF)
    {
        return mem->mbc.rom_read(mem, addr);
    }
    else if (addr <= 0x9FFF)
    {
        // if this ever evolves to CGB, vram has switchable banks 0/1
        return mem->vram[addr - 0x8000];
    }
    else if (addr <= 0xBFFF)
    {
        return mem->mbc.ram_read(mem, addr);
    }
    else if (addr <= 0xCFFF)
    {
        return mem->wram0[addr - 0xC000];
    }
    else if (addr <= 0xDFFF)
    {
        // if this ever evolves to CGB, wram1 has switchable banks 1-7
        return mem->wram1[addr - 0xD000];
    }
    else if (addr <= 0xFDFF)
    {
        uint16_t mirrored = addr - 0x2000;

        if (mirrored > 0xCFFF)
            return mem->wram1[mirrored - 0xD000];

        return mem->wram0[mirrored - 0xC000];
    }
    else if (addr <= 0xFE9F)
    {
        return mem->oam[addr - 0xFE00];
    }
    else if (addr <= 0xFEFF)
    {
        return 0xFF;
    }
    else if (addr <= 0xFF7F)
    {
        return mem->io[addr - 0xFF00];
    }
    else if (addr <= 0xFFFE)
    {
        return mem->hram[addr - 0xFF80];
    }
    else if (addr == 0xFFFF)
    {
        return mem->ie;
    }

    return 0xFF;
}

void memory_write16(Memory* mem, uint16_t addr, uint16_t value)
{
    memory_write(mem, addr, value & 0xFF);
    memory_write(mem, addr + 1, (value >> 8) & 0xFF);
}

uint16_t memory_read16(Memory* mem, uint16_t addr)
{
    uint8_t lo = memory_read(mem, addr);
    uint8_t hi = memory_read(mem, addr + 1);

    return (hi << 8) | lo;
}

Memory* memory_init()
{
    Memory *mem = (Memory*) malloc(sizeof(Memory));     
    memset(mem, 0, sizeof(Memory));

    set_mbc_type(mem, MBC1);

    return mem;
}

void set_mbc_type(Memory* mem, MBCType type)
{
    if (mem->mbc.mbc_type == type)
        return;

    switch (type)
    {
        case MBC_NONE: {
            mem->mbc.mbc_type = MBC_NONE;
            mem->mbc.rom_write = rom_write_mbc_none;
            mem->mbc.rom_read = rom_read_mbc_none;
            mem->mbc.ram_write = ram_write_mbc_none;
            mem->mbc.ram_read = ram_read_mbc_none;
            break;
        }
        case MBC1: {
            mem->mbc.mbc_type = MBC1;
            mem->mbc.rom_write = rom_write_mbc1;
            mem->mbc.rom_read = rom_read_mbc1;
            mem->mbc.ram_write = ram_write_mbc1;
            mem->mbc.ram_read = ram_read_mbc1;
            break;
        }
    }
}
