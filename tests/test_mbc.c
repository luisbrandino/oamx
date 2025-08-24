#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../inc/memory.h"

void test_mbc1_rom_bank_switch_no_banking_mode()
{
    Memory *mem = memory_init();
    
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

void test_mbc1_ram_enable()
{
    Memory *mem = memory_init();

    memory_write(mem, 0x0000, 0x0A);
    assert(mem->mbc.ram_enabled == 1);

    memory_write(mem, 0x0000, 0x00);
    assert(mem->mbc.ram_enabled == 0);

    free(mem);
}

int main()
{
    printf("Running MBC tests...\n");

    test_mbc1_rom_bank_switch_no_banking_mode();
    test_mbc1_ram_enable();

    printf("All MBC tests passed!\n");

    return 0;
}