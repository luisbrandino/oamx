#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../inc/memory.h"

void test_memory_vram_write_and_read()
{
    Memory *mem = memory_init();

    memory_write(mem, 0x8000, 0x1C);
    assert(memory_read(mem, 0x8000) == 0x1C);
    assert(mem->vram[0x0000] == 0x1C);

    free(mem);
}

void test_memory_wram0_write_and_read()
{
    Memory *mem = memory_init();

    memory_write(mem, 0xC000, 0x1C);
    assert(memory_read(mem, 0xC000) == 0x1C);
    assert(mem->wram0[0x0000] == 0x1C);

    free(mem);
}

void test_memory_wram1_write_and_read()
{
    Memory *mem = memory_init();

    memory_write(mem, 0xD000, 0x1C);
    assert(memory_read(mem, 0xD000) == 0x1C);
    assert(mem->wram1[0x0000] == 0x1C);

    free(mem);
}

void test_memory_echo_ram_write_and_read()
{
    Memory *mem = memory_init();

    memory_write(mem, 0xE000, 0x1C);
    assert(memory_read(mem, 0xE000) == 0x1C);
    assert(memory_read(mem, 0xC000) == 0x1C);
    assert(mem->wram0[0x0000] == 0x1C);

    memory_write(mem, 0xF000, 0x1C);
    assert(memory_read(mem, 0xF000) == 0x1C);
    assert(memory_read(mem, 0xD000) == 0x1C);
    assert(mem->wram1[0x0000] == 0x1C);

    free(mem);
}

void test_memory_oam_write_and_read()
{
    Memory *mem = memory_init();

    memory_write(mem, 0xFE00, 0x1C);
    assert(memory_read(mem, 0xFE00) == 0x1C);
    assert(mem->oam[0x0000] == 0x1C);

    free(mem);
}

void test_memory_not_usable_area_read()
{
    Memory *mem = memory_init();

    assert(memory_read(mem, 0xFEA0) == 0xFF);

    free(mem);
}

void test_memory_io_write_and_read()
{
    Memory *mem = memory_init();

    memory_write(mem, 0xFF00, 0x1C);
    assert(memory_read(mem, 0xFF00) == 0x1C);
    assert(mem->io[0x0000] == 0x1C);

    free(mem);
}

void test_memory_hram_write_and_read()
{
    Memory *mem = memory_init();

    memory_write(mem, 0xFF80, 0x1C);
    assert(memory_read(mem, 0xFF80) == 0x1C);
    assert(mem->hram[0x0000] == 0x1C);

    free(mem);
}

void test_memory_ie_write_and_read()
{
    Memory *mem = memory_init();

    memory_write(mem, 0xFFFF, 0x01);
    assert(memory_read(mem, 0xFFFF) == 0x01);
    assert(mem->ie == 0x01);

    free(mem);
}

void test_memory_write16_and_read16()
{
    Memory *mem = memory_init();

    memory_write16(mem, 0x8000, 0x2080);
    assert(memory_read16(mem, 0x8000) == 0x2080);
    assert(mem->vram[0x0000] == 0x80);
    assert(mem->vram[0x0001] == 0x20);

    free(mem);
}

int main()
{
    test_memory_vram_write_and_read();
    test_memory_wram0_write_and_read();
    test_memory_wram1_write_and_read();
    test_memory_echo_ram_write_and_read();
    test_memory_oam_write_and_read();
    test_memory_not_usable_area_read();
    test_memory_io_write_and_read();
    test_memory_hram_write_and_read();
    test_memory_ie_write_and_read();
    test_memory_write16_and_read16();

    return EXIT_SUCCESS;
}