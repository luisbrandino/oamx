#include "../inc/cpu.h"
#include "../inc/memory.h"
#include <stdlib.h>
#include <string.h>

Cpu* cpu_init()
{
    Cpu* cpu = (Cpu*)malloc(sizeof(Cpu));
    memset(cpu, 0, sizeof(Cpu));

    // if a BIOS is loaded we shouldn't call this
    cpu_reset(cpu);

    return cpu;
}

// set CPU registers to default post-BIOS values
// if a BIOS is loaded, these will be overriden by the BIOS execution
void cpu_reset(Cpu* cpu)
{
    set_af(cpu, 0x01B0);
    set_bc(cpu, 0x0013);
    set_de(cpu, 0x00D8);
    set_hl(cpu, 0x014D);

    cpu->sp = 0xFFFE; // some homebrews may use 0xDFFF (wram) as the top of the stack
    cpu->pc = 0x0100;
}

uint8_t cpu_step(Cpu* cpu, Memory* mem)
{

}