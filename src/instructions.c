#include "../inc/instructions.h"

// --- HELPERS --- //

uint8_t increment(Cpu* cpu, uint8_t value)
{
    if ((value & 0x0F) == 0x0F)
        SET_FLAG(FLAG_HALFCARRY);
    else
        CLEAR_FLAG(FLAG_HALFCARRY);

    value++;

    if (value == 0)
        SET_FLAG(FLAG_ZERO);
    else
        CLEAR_FLAG(FLAG_ZERO);

    CLEAR_FLAG(FLAG_NEGATIVE);

    return value;
}

// --- INSTRUCTIONS --- //

void nop(Cpu* cpu, Memory* mem) { }

void ld_bc_nn(Cpu* cpu, Memory* mem)
{
    set_bc(cpu, memory_read16(mem, cpu->pc));
}

void ld_at_bc_a(Cpu* cpu, Memory* mem)
{
    memory_write(mem, get_bc(cpu), cpu->a);
}

void inc_bc(Cpu* cpu, Memory* mem)
{
    set_bc(cpu, get_bc(cpu) + 1);
}

void inc_b(Cpu* cpu, Memory* mem)
{
    cpu->b = increment(cpu, cpu->b);
}

void call_nn(Cpu* cpu, Memory* mem)
{
    cpu_push(cpu, mem, cpu->pc + 2);
    cpu->pc = memory_read16(mem, cpu->pc);
}

const Instruction instructions[0x100] = {
    { "NOP",        0, 4,   PC_ADVANCE, nop },
    { "LD BC, nn",  2, 12,  PC_ADVANCE, ld_bc_nn },
    { "LD [BC], A", 0, 8,   PC_ADVANCE, ld_at_bc_a },
    { "INC BC",     0, 8,   PC_ADVANCE, inc_bc },
    { "INC BC",     0, 4,   PC_ADVANCE, inc_b },
};

uint8_t execute_instruction(Cpu* cpu, Memory* mem, uint8_t byte)
{
    Instruction instruction = instructions[byte];

    instruction.handle(cpu, mem);

    if (instructions->pc_mode == PC_ADVANCE)
        cpu_advance_pc(cpu, instruction.operand_length);

    return instruction.ticks;
}