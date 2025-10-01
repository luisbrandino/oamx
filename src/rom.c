#include <stdio.h>
#include "assert.h"

#include "../inc/rom.h"

void load_rom(Memory* mem, char* filename)
{   
    FILE* f;

    f = fopen(filename, "rb");
    assert(f != NULL);

    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    assert(size >= 0x180);
    assert(size <= sizeof(mem->rom));
    rewind(f);

    size_t bytes_read = fread(mem->rom, 1, size, f);
    assert(bytes_read == size);

    fclose(f);
}