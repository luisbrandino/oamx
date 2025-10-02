#include "../inc/memory.h"
#include <stdlib.h>
#include <string.h>

void memory_write(Memory* mem, uint16_t addr, uint8_t value)
{
    switch (addr)
    {
        case JOYP_ADDR:
            mem->joyp = (mem->joyp & 0x0F) | (value & 0x30) | 0xC0;
            break;
        case DIV_ADDR:
            mem->div = value;
            break;
        case TIMA_ADDR:
            mem->tima = value;
            break;
        case TMA_ADDR:
            mem->tma = value;
            break;
        case TAC_ADDR:
            mem->tac = value;
            break;
        case LCDC_ADDR:
            mem->lcdc = value;
            break;
        case STAT_ADDR:
            mem->stat = value;
            break;
        case LY_ADDR:
            mem->ly = value;
            break;
        case LYC_ADDR:
            mem->lyc = value;
            break;
        case SCY_ADDR:
            mem->scy = value;
            break;
        case SCX_ADDR:
            mem->scx = value;
            break;
        case BGP_ADDR:
            mem->bgp = value;
            break;
        case OBP0_ADDR:
            mem->obp0 = value;
            break;
        case OBP1_ADDR:
            mem->obp1 = value;
            break;
        case WY_ADDR:
            mem->wy = value;
            break;
        case WX_ADDR:
            mem->wx = value;
            break;
        case IE_ADDR:
            mem->IE = value;
            break;
        case IF_ADDR:
            mem->IF = value;
            break;
    }

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
        if (addr == 0xFF46)
        {
            mem->io[0x46] = value;
            for (uint16_t i = 0; i < 0xA0; i++)
                mem->oam[i] = memory_read(mem, (value << 8) + i);
            return;
        }

        mem->io[addr - 0xFF00] = value;
    }
    else if (addr <= 0xFFFE)
    {
        mem->hram[addr - 0xFF80] = value;
    }
}

uint8_t memory_read(Memory* mem, uint16_t addr)
{
    switch (addr)
    {
        case JOYP_ADDR:
            if (!(mem->joyp & 0x20))
                return (mem->joypad_state >> 4) | (1 << 4);

            if (!(mem->joyp & 0x10))
                return (mem->joypad_state & 0x0F) | (1 << 5);

            return mem->joyp | 0xFF;
        case DIV_ADDR:
            return mem->div;
        case TIMA_ADDR:
            return mem->tima;
        case TMA_ADDR:
            return mem->tma;
        case TAC_ADDR:
            return mem->tac;
        case LCDC_ADDR:
            return mem->lcdc;
        case STAT_ADDR:
            return mem->stat;
        case LY_ADDR:
            return mem->ly;
        case LYC_ADDR:
            return mem->lyc;
        case SCY_ADDR:
            return mem->scy;
        case SCX_ADDR:
            return mem->scx;
        case BGP_ADDR:
            return mem->bgp;
        case OBP0_ADDR:
            return mem->obp0;
        case OBP1_ADDR:
            return mem->obp1;
        case WY_ADDR:
            return mem->wy;
        case WX_ADDR:
            return mem->wx;
        case IE_ADDR:
            return mem->IE;
        case IF_ADDR:
            return mem->IF;
    }

    if (addr <= 0x7FFF)
    {
        return mem->mbc.rom_read(mem, addr);
    }
    else if (addr <= 0x9FFF)
    {
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

void memory_reset(Memory* mem)
{
    mem->joypad_state = 0xFF;
    mem->div = 0x18;
    mem->lcdc = 0x91;
    mem->stat = 0x85;
    mem->bgp = 0xFC;
    mem->IF = 0xE1;
}

Memory* memory_init()
{
    Memory *mem = (Memory*) malloc(sizeof(Memory));     
    memset(mem, 0, sizeof(Memory));
    memory_reset(mem);

    set_mbc_type(mem, MBC1);

    return mem;
}

void set_mbc_type(Memory* mem, MBCType type)
{
    switch (type)
    {
        case MBC_NONE:
            mem->mbc.mbc_type = MBC_NONE;
            mem->mbc.rom_write = rom_write_mbc_none;
            mem->mbc.rom_read = rom_read_mbc_none;
            mem->mbc.ram_write = ram_write_mbc_none;
            mem->mbc.ram_read = ram_read_mbc_none;
            break;
        case MBC1:
            mem->mbc.mbc_type = MBC1;
            mem->mbc.rom_write = rom_write_mbc1;
            mem->mbc.rom_read = rom_read_mbc1;
            mem->mbc.ram_write = ram_write_mbc1;
            mem->mbc.ram_read = ram_read_mbc1;
            break;
    }
}