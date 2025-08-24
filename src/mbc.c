#include "../inc/mbc.h"
#include "../inc/memory.h"

// --- MBC1 and MBC_NONE implementations --- //

void rom_write_mbc_none(Memory* memory, uint16_t addr, uint8_t value)
{
    // no writes to rom for mbc none
}

uint8_t rom_read_mbc_none(Memory* memory, uint16_t addr)
{
    return memory->rom[addr];    
}

// some cartridges without an MBC chip may include a fixed 8 KB SRAM at $A000–$BFFF.
// in this case, the game code can read/write this RAM, even without banking logic.
// most MBC-less cartridges, however, do not have external RAM, so accesses to this area would do nothing.
void ram_write_mbc_none(Memory* memory, uint16_t addr, uint8_t value)
{
    if (addr < 0xA000 || addr > 0xBFFF)
        return;

    memory->sram[addr - 0xA000] = value;
}

uint8_t ram_read_mbc_none(Memory* memory, uint16_t addr)
{
    if (addr < 0xA000 || addr > 0xBFFF)
        return 0xFF;

    return memory->sram[addr - 0xA000];
}

void rom_write_mbc1(Memory* memory, uint16_t addr, uint8_t value)
{
    if (addr <= 0x1FFF)
    {
        memory->mbc.ram_enabled = (value & 0x0F) == 0x0A;
    }
    else if (addr <= 0x3FFF)
    {
        uint8_t rom_size_code = memory->rom[0x148];
        uint8_t mask = 2 << rom_size_code;

        memory->mbc.rom_bank = (value & 0x1F) & mask;

        // for MBC1, bank 0 is normally translated to bank 1 in the switchable ROM area.
        // however, the original MBC1 chips have a 5-bit trick: if a cartridge uses only 4 bits (16 banks),
        // the highest bit (5th bit) is ignored and switching to bank 0 would be possible in this scenario.
        // e.g.: 0b10000 -> 0b0000
        // this simplification ignores it.
        memory->mbc.rom_bank = memory->mbc.rom_bank == 0 ? 1 : memory->mbc.rom_bank;
    }
    else if (addr <= 0x5FFF)
    {
        memory->mbc.ram_bank = value & 0x03;
    }
    else if (addr <= 0x7FFF)
    {
        memory->mbc.banking_mode = value & 0x01;
    }
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

void ram_write_mbc1(Memory* memory, uint16_t addr, uint8_t value)
{
    if (addr < 0xA000 || addr > 0xBFFF)
        return;

    if (memory->mbc.ram_enabled == 0)
        return;

    uint16_t offset = memory->mbc.ram_bank * 0x2000 + (addr - 0xA000);

    memory->sram[offset] = value;
}

uint8_t ram_read_mbc1(Memory* memory, uint16_t addr)
{
    if (addr < 0xA000 || addr > 0xBFFF)
        return 0xFF;

    if (memory->mbc.ram_enabled == 0)
        return 0xFF;

    uint16_t offset = memory->mbc.ram_bank * 0x2000 + (addr - 0xA000);
    
    return memory->sram[offset];
}