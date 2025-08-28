#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../inc/memory.h"

void test_mbc_none_rom_read()
{
    Memory *mem = memory_init();
    set_mbc_type(mem, MBC_NONE);

    mem->rom[0x1000] = 0x1C;

    assert(memory_read(mem, 0x1000) == 0x1C);

    free(mem);
}

void test_mbc_none_ram_read()
{
    Memory *mem = memory_init();
    set_mbc_type(mem, MBC_NONE);

    mem->sram[0x0001] = 0x1C;

    assert(memory_read(mem, 0xA001) == 0x1C);

    free(mem);
}

void test_mbc1_rom_read()
{
    Memory *mem = memory_init();
    set_mbc_type(mem, MBC1);

    for (size_t i = 0; i < 5; i++)
    {
        mem->rom[0x148] = i;

        uint8_t total_banks = 2 << i;

        for (size_t bank = 1; bank < total_banks; bank++)
        {
            memory_write(mem, 0x2000, bank);
            
            uint16_t addr = 0x5000;

            mem->rom[0x4000 * bank + (addr - 0x4000)] = 0x1C;

            assert(memory_read(mem, addr) == 0x1C);

            mem->rom[0x4000 * bank + (addr - 0x4000)] = 0x00;
        }
    }

    // setting ram banking mode 0 
    memory_write(mem, 0x6000, 0x00);

    for (size_t ram_bank = 0; ram_bank < 4; ram_bank++)
    {
        // setting ram bank to ram_bank
        memory_write(mem, 0x4000, ram_bank);

        for (size_t i = 0; i < 5; i++)
        {
            mem->rom[0x148] = i;

            uint8_t total_banks = 2 << i;

            for (size_t bank = 1; bank < total_banks; bank++)
            {
                memory_write(mem, 0x2000, bank);
                memory_write(mem, 0x4000, ram_bank);

                uint8_t combined_bank = (ram_bank << 5) | bank;
                assert(mem->mbc.rom_bank == combined_bank);

                uint16_t addr = 0x5000;

                mem->rom[0x4000 * combined_bank + (addr - 0x4000)] = 0x1C;

                assert(memory_read(mem, addr) == 0x1C);

                mem->rom[0x4000 * combined_bank + (addr - 0x4000)] = 0x0;
            }
        }
    }

    free(mem);
}

void test_mbc1_ram_read()
{
    Memory *mem = memory_init();
    set_mbc_type(mem, MBC1);

    // disable ram
    memory_write(mem, 0x0000, 0x00);

    assert(memory_read(mem, 0xA000) == 0xFF);

    // enable ram
    memory_write(mem, 0x0000, 0x0A);

    // set banking mode to 1
    memory_write(mem, 0x6000, 0x01);

    for (size_t ram_bank = 0; ram_bank < 4; ram_bank++)
    {
        memory_write(mem, 0x4000, ram_bank);
        assert(mem->mbc.ram_bank == ram_bank);

        uint16_t addr = 0xA001;

        mem->sram[0x2000 * ram_bank + (addr - 0xA000)] = 0x1C;
        assert(memory_read(mem, addr) == 0x1C);
        mem->sram[0x2000 * ram_bank + (addr - 0xA000)] = 0x0;
    }

    free(mem);
}

void test_mbc1_rom_bank_switch_no_banking_mode()
{
    Memory *mem = memory_init();
    set_mbc_type(mem, MBC1);
    
    for (size_t i = 0; i < 5; i++)
    {
        mem->rom[0x148] = i;

        uint8_t total_banks = 2 << i;

        for (size_t j = 0; j < total_banks*2; j++)
        {
            memory_write(mem, 0x2000, j);
            assert(mem->mbc.rom_bank == ((j % total_banks > 0) ? (j % total_banks) : 1));
        }
    }

    memory_write(mem, 0x2000, 0x0);
    assert(mem->mbc.rom_bank == 0x1);

    free(mem);
}

void test_mbc1_banking_mode_enable()
{
    Memory *mem = memory_init();
    set_mbc_type(mem, MBC1);

    memory_write(mem, 0x6000, 0x01);
    assert(mem->mbc.banking_mode == 1);

    memory_write(mem, 0x6000, 0x00);
    assert(mem->mbc.banking_mode == 0);

    free(mem);
}

void test_mbc1_ram_enable()
{
    Memory *mem = memory_init();
    set_mbc_type(mem, MBC1);

    memory_write(mem, 0x0000, 0x0A);
    assert(mem->mbc.ram_enabled == 1);

    memory_write(mem, 0x0000, 0x00);
    assert(mem->mbc.ram_enabled == 0);

    free(mem);
}

void test_mbc1_ram_bank_switch()
{
    Memory *mem = memory_init();
    set_mbc_type(mem, MBC1);

    // setting ram banking mode 1 
    memory_write(mem, 0x6000, 0x01);
    
    for (size_t i = 0; i < 4; i++)
    {
        memory_write(mem, 0x4000, i);
        assert(mem->mbc.ram_bank == i);  
    }

    free(mem);
}

int main()
{
    test_mbc_none_rom_read();
    test_mbc_none_ram_read();
    test_mbc1_rom_read();
    test_mbc1_ram_read();
    test_mbc1_rom_bank_switch_no_banking_mode();
    test_mbc1_banking_mode_enable();
    test_mbc1_ram_enable();
    test_mbc1_ram_bank_switch();

    return EXIT_SUCCESS;
}