#include "../inc/cb_instructions.h"

// --- HELPERS --- //

uint8_t rlc(Cpu* cpu, uint8_t value)
{
    uint8_t carry_bit = value >> 7;
    if (carry_bit)
        SET_FLAG(FLAG_CARRY);
    else
        CLEAR_FLAG(FLAG_CARRY);
    
    value = (value << 1) | carry_bit;
    if (value == 0)
        SET_FLAG(FLAG_ZERO);
    else
        CLEAR_FLAG(FLAG_ZERO);

    CLEAR_FLAG(FLAG_NEGATIVE | FLAG_HALFCARRY);

    return value;
}

uint8_t rrc(Cpu* cpu, uint8_t value)
{
    uint8_t carry_bit = value & 0x01;
    if (carry_bit)
        SET_FLAG(FLAG_CARRY);
    else
        CLEAR_FLAG(FLAG_CARRY);
    
    value = (value >> 1) | (carry_bit << 7);
    if (value == 0)
        SET_FLAG(FLAG_ZERO);
    else
        CLEAR_FLAG(FLAG_ZERO);

    CLEAR_FLAG(FLAG_NEGATIVE | FLAG_HALFCARRY);

    return value;
}

uint8_t rl(Cpu* cpu, uint8_t value)
{
    uint8_t carry_in = IS_FLAG_SET(FLAG_CARRY);
    uint8_t carry_out = value >> 7;
    if (carry_out)
        SET_FLAG(FLAG_CARRY);
    else
        CLEAR_FLAG(FLAG_CARRY);

    value = (value << 1) | carry_in;
    if (value == 0)
        SET_FLAG(FLAG_ZERO);
    else
        CLEAR_FLAG(FLAG_ZERO);

    CLEAR_FLAG(FLAG_NEGATIVE | FLAG_HALFCARRY);

    return value;
}

uint8_t rr(Cpu* cpu, uint8_t value)
{
    uint8_t carry_in = IS_FLAG_SET(FLAG_CARRY);
    uint8_t carry_out = value & 0x01;
    if (carry_out)
        SET_FLAG(FLAG_CARRY);
    else
        CLEAR_FLAG(FLAG_CARRY);
    
    value = (value >> 1) | (carry_in << 7);
    if (value == 0)
        SET_FLAG(FLAG_ZERO);
    else
        CLEAR_FLAG(FLAG_ZERO);
    
    CLEAR_FLAG(FLAG_NEGATIVE | FLAG_HALFCARRY);

    return value;
}

// --- INSTRUCTIONS --- //

void rlc_b(Cpu* cpu, Memory* mem) { cpu->b = rlc(cpu, cpu->b); }
void rlc_c(Cpu* cpu, Memory* mem) { cpu->c = rlc(cpu, cpu->c); }
void rlc_d(Cpu* cpu, Memory* mem) { cpu->d = rlc(cpu, cpu->d); }
void rlc_e(Cpu* cpu, Memory* mem) { cpu->e = rlc(cpu, cpu->e); }
void rlc_h(Cpu* cpu, Memory* mem) { cpu->h = rlc(cpu, cpu->h); }
void rlc_l(Cpu* cpu, Memory* mem) { cpu->l = rlc(cpu, cpu->l); }
void rlc_at_hl(Cpu* cpu, Memory* mem) { memory_write(mem, get_hl(cpu), rlc(cpu, memory_read(mem, get_hl(cpu)))); }
void rlc_a(Cpu* cpu, Memory* mem) { cpu->a = rlc(cpu, cpu->a); }

void rrc_b(Cpu* cpu, Memory* mem) { cpu->b = rrc(cpu, cpu->b); }
void rrc_c(Cpu* cpu, Memory* mem) { cpu->c = rrc(cpu, cpu->c); }
void rrc_d(Cpu* cpu, Memory* mem) { cpu->d = rrc(cpu, cpu->d); }
void rrc_e(Cpu* cpu, Memory* mem) { cpu->e = rrc(cpu, cpu->e); }
void rrc_h(Cpu* cpu, Memory* mem) { cpu->h = rrc(cpu, cpu->h); }
void rrc_l(Cpu* cpu, Memory* mem) { cpu->l = rrc(cpu, cpu->l); }
void rrc_at_hl(Cpu* cpu, Memory* mem) { memory_write(mem, get_hl(cpu), rrc(cpu, memory_read(mem, get_hl(cpu)))); }
void rrc_a(Cpu* cpu, Memory* mem) { cpu->a = rrc(cpu, cpu->a); }

void rl_b(Cpu* cpu, Memory* mem) { cpu->b = rl(cpu, cpu->b); }
void rl_c(Cpu* cpu, Memory* mem) { cpu->c = rl(cpu, cpu->c); }
void rl_d(Cpu* cpu, Memory* mem) { cpu->d = rl(cpu, cpu->d); }
void rl_e(Cpu* cpu, Memory* mem) { cpu->e = rl(cpu, cpu->e); }
void rl_h(Cpu* cpu, Memory* mem) { cpu->h = rl(cpu, cpu->h); }
void rl_l(Cpu* cpu, Memory* mem) { cpu->l = rl(cpu, cpu->l); }
void rl_at_hl(Cpu* cpu, Memory* mem) { memory_write(mem, get_hl(cpu), rl(cpu, memory_read(mem, get_hl(cpu)))); }
void rl_a(Cpu* cpu, Memory* mem) { cpu->a = rl(cpu, cpu->a); }

void rr_b(Cpu* cpu, Memory* mem) { cpu->b = rr(cpu, cpu->b); }
void rr_c(Cpu* cpu, Memory* mem) { cpu->c = rr(cpu, cpu->c); }
void rr_d(Cpu* cpu, Memory* mem) { cpu->d = rr(cpu, cpu->d); }
void rr_e(Cpu* cpu, Memory* mem) { cpu->e = rr(cpu, cpu->e); }
void rr_h(Cpu* cpu, Memory* mem) { cpu->h = rr(cpu, cpu->h); }
void rr_l(Cpu* cpu, Memory* mem) { cpu->l = rr(cpu, cpu->l); }
void rr_at_hl(Cpu* cpu, Memory* mem) { memory_write(mem, get_hl(cpu), rr(cpu, memory_read(mem, get_hl(cpu)))); }
void rr_a(Cpu* cpu, Memory* mem) { cpu->a = rr(cpu, cpu->a); }

const CBInstruction cb_instructions[0x100] = {
    { "RLC B",      8, rlc_b },
    { "RLC C",      8, rlc_c },
    { "RLC D",      8, rlc_d },
    { "RLC E",      8, rlc_e },
    { "RLC H",      8, rlc_h },
    { "RLC L",      8, rlc_l },
    { "RLC [HL]",  16, rlc_at_hl },
    { "RLC A",      8, rlc_a },
    { "RRC B",      8, rrc_b },
    { "RRC C",      8, rrc_c },
    { "RRC D",      8, rrc_d },
    { "RRC E",      8, rrc_e },
    { "RRC H",      8, rrc_h },
    { "RRC L",      8, rrc_l },
    { "RRC [HL]",  16, rrc_at_hl },
    { "RRC A",      8, rrc_a },
    { "RL B",       8, rl_b },
    { "RL C",       8, rl_c },
    { "RL D",       8, rl_d },
    { "RL E",       8, rl_e },
    { "RL H",       8, rl_h },
    { "RL L",       8, rl_l },
    { "RL [HL]",   16, rl_at_hl },
    { "RL A",       8, rl_a },
    { "RR B",       8, rr_b },
    { "RR C",       8, rr_c },
    { "RR D",       8, rr_d },
    { "RR E",       8, rr_e },
    { "RR H",       8, rr_h },
    { "RR L",       8, rr_l },
    { "RR [HL]",   16, rr_at_hl },
    { "RR A",       8, rr_a },
};

void execute_cb_instruction(Cpu* cpu, Memory* mem, uint8_t byte)
{
    if (byte < 0 || byte > 0x100)
        return;

    CBInstruction cb_instruction = cb_instructions[byte];
    cb_instruction.handle(cpu, mem);
    cpu_add_ticks(cpu, cb_instruction.ticks);
}