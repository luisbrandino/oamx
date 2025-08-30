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

void cpu_push(Cpu* cpu, Memory* mem, uint16_t value)
{
    cpu->sp -= 2;
    memory_write16(mem, cpu->sp, value);
}

uint16_t cpu_pop(Cpu* cpu, Memory* mem)
{
    uint16_t value = memory_read16(mem, cpu->sp);
    cpu->sp += 2;
    return value;
}

void cpu_call(Cpu* cpu, Memory* mem, uint16_t addr)
{
    cpu_push(cpu, mem, cpu->pc);
    cpu->pc = addr;
}

void cpu_ret(Cpu* cpu, Memory* mem)
{
    cpu->pc = cpu_pop(cpu, mem);
}

uint8_t cpu_step(Cpu* cpu, Memory* mem)
{
    // to be implemented
}