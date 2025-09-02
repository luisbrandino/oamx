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

uint16_t add16(Cpu* cpu, uint16_t src, uint16_t value)
{
    uint32_t result = src + value;

    if (result & 0xFFFF0000)
        SET_FLAG(FLAG_CARRY);
    else
        CLEAR_FLAG(FLAG_CARRY);

    if ((src & 0x0FFF) + (value & 0x0FFF) > 0x0FFF)
        SET_FLAG(FLAG_HALFCARRY);
    else
        CLEAR_FLAG(FLAG_HALFCARRY);

    src = (uint16_t)(result & 0xFFFF);

    CLEAR_FLAG(FLAG_NEGATIVE);

    return src;
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
    uint8_t carry_bit = cpu->a >> 7;
    if (carry_bit)
        SET_FLAG(FLAG_CARRY);
    else
        CLEAR_FLAG(FLAG_CARRY);

    cpu->a <<= 1;
    cpu->a += carry_bit;

    CLEAR_FLAG(FLAG_NEGATIVE | FLAG_ZERO | FLAG_HALFCARRY);
}

void ld_at_nn_sp(Instruction* instr, Cpu* cpu, Memory* mem)
{
    memory_write16(mem, instr->operand16, cpu->sp);
}

void add_hl_bc(Instruction* instr, Cpu* cpu, Memory* mem)
{
    uint16_t hl = get_hl(cpu);
    uint16_t bc = get_bc(cpu);

    set_hl(cpu, add16(cpu, hl, bc));
}

void ld_a_at_bc(Instruction* instr, Cpu* cpu, Memory* mem)
{
    cpu->a = memory_read(mem, get_bc(cpu));
}

void dec_bc(Instruction* instr, Cpu* cpu, Memory* mem)
{
    set_bc(cpu, get_bc(cpu) - 1);
}

void inc_c(Instruction* instr, Cpu* cpu, Memory* mem)
{
    cpu->c = increment(cpu, cpu->c);
}

void dec_c(Instruction* instr, Cpu* cpu, Memory* mem)
{
    cpu->c = decrement(cpu, cpu->c);
}

void ld_c_n(Instruction* instr, Cpu* cpu, Memory* mem)
{
    cpu->c = instr->operand8;
}

void rrca(Instruction* instr, Cpu* cpu, Memory* mem)
{
    uint8_t carry_bit = cpu->a & 0x01;
    if (carry_bit)
        SET_FLAG(FLAG_CARRY);
    else
        CLEAR_FLAG(FLAG_CARRY);

    cpu->a >>= 1;
    cpu->a |= (carry_bit << 7);

    CLEAR_FLAG(FLAG_NEGATIVE | FLAG_ZERO | FLAG_HALFCARRY);
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
    { "ADD HL, BC",     8,  OPERAND_NONE, PC_ADVANCE, .handle = add_hl_bc },
    { "LD A, [BC]",     8,  OPERAND_NONE, PC_ADVANCE, .handle = ld_a_at_bc },
    { "DEC BC",         8,  OPERAND_NONE, PC_ADVANCE, .handle = dec_bc },
    { "INC C",          4,  OPERAND_NONE, PC_ADVANCE, .handle = inc_c },
    { "DEC C",          4,  OPERAND_NONE, PC_ADVANCE, .handle = dec_c },
    { "LD C, n",        8,  OPERAND_BYTE, PC_ADVANCE, .handle = ld_c_n },
    { "RRCA",           4,  OPERAND_NONE, PC_ADVANCE, .handle = rrca },
};

void instruction_execute(Cpu* cpu, Memory* mem, uint8_t byte)
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