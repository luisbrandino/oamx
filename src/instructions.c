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

uint8_t decrement(Cpu* cpu, uint8_t value)
{
    if (value & 0x0F)
        CLEAR_FLAG(FLAG_HALFCARRY);
    else
        SET_FLAG(FLAG_HALFCARRY);

    value--;

    if (value == 0)
        SET_FLAG(FLAG_ZERO);
    else
        CLEAR_FLAG(FLAG_ZERO);
    
    SET_FLAG(FLAG_NEGATIVE);

    return value;
}

// --- INSTRUCTIONS --- //

void nop(Instruction* instr, Cpu* cpu, Memory* mem) { }

void ld_bc_nn(Instruction* instr, Cpu* cpu, Memory* mem)
{
    set_bc(cpu, instr->operand16);
}

void ld_at_bc_a(Instruction* instr, Cpu* cpu, Memory* mem)
{
    memory_write(mem, get_bc(cpu), cpu->a);
}

void inc_bc(Instruction* instr, Cpu* cpu, Memory* mem)
{
    set_bc(cpu, get_bc(cpu) + 1);
}

void inc_b(Instruction* instr, Cpu* cpu, Memory* mem)
{
    cpu->b = increment(cpu, cpu->b);
}

void dec_b(Instruction* instr, Cpu* cpu, Memory* mem)
{
    cpu->b = decrement(cpu, cpu->b);
}

void ld_b_n(Instruction* instr, Cpu* cpu, Memory* mem)
{
    cpu->b = instr->operand8;
}

void rlca(Instruction* instr, Cpu* cpu, Memory* mem)
{
    uint8_t last_bit = cpu->a >> 7;
    if (last_bit)
        SET_FLAG(FLAG_CARRY);
    else
        CLEAR_FLAG(FLAG_CARRY);

    cpu->a <<= 1;
    cpu->a += last_bit;

    CLEAR_FLAG(FLAG_NEGATIVE | FLAG_ZERO | FLAG_HALFCARRY);
}

void ld_at_nn_sp(Instruction* instr, Cpu* cpu, Memory* mem)
{
    memory_write16(mem, instr->operand16, cpu->sp);
}

void call_nn(Instruction* instr, Cpu* cpu, Memory* mem)
{
    cpu_push(cpu, mem, cpu->pc + 2);
    cpu->pc = instr->operand16;
}

const Instruction instructions[0x100] = {
    { "NOP",            4,  OPERAND_NONE, PC_ADVANCE, .handle = nop },
    { "LD BC, nn",      12, OPERAND_WORD, PC_ADVANCE, .handle = ld_bc_nn },
    { "LD [BC], A",     8,  OPERAND_NONE, PC_ADVANCE, .handle = ld_at_bc_a },
    { "INC BC",         8,  OPERAND_NONE, PC_ADVANCE, .handle = inc_bc },
    { "INC B",          4,  OPERAND_NONE, PC_ADVANCE, .handle = inc_b },
    { "DEC B",          4,  OPERAND_NONE, PC_ADVANCE, .handle = dec_b },
    { "LD B, n",        8,  OPERAND_BYTE, PC_ADVANCE, .handle = ld_b_n },
    { "RLCA",           4,  OPERAND_NONE, PC_ADVANCE, .handle = rlca },
    { "LD [nn], SP",    20, OPERAND_WORD, PC_ADVANCE, .handle = ld_at_nn_sp },
};

void execute_instruction(Cpu* cpu, Memory* mem, uint8_t byte)
{
    Instruction instruction = instructions[byte];

    instruction.operand8 = 0;
    instruction.operand16 = 0;

    switch (instruction.operand_size)
    {
        case OPERAND_BYTE:
            instruction.operand8 = memory_read(mem, cpu->pc);
            break;
        case OPERAND_WORD:
            instruction.operand16 = memory_read16(mem, cpu->pc);
            break;
    };

    instruction.handle(&instruction, cpu, mem);

    if (instructions->pc_mode == PC_ADVANCE)
        cpu_advance_pc(cpu, instruction.operand_size);

    cpu_add_ticks(cpu, instruction.base_ticks);
}