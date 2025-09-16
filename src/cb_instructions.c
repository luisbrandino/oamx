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

uint8_t sla(Cpu* cpu, uint8_t value)
{
    if (value >> 7)
        SET_FLAG(FLAG_CARRY);
    else
        CLEAR_FLAG(FLAG_CARRY);

    value <<= 1;
    if (value == 0)
        SET_FLAG(FLAG_ZERO);
    else
        CLEAR_FLAG(FLAG_ZERO);

    CLEAR_FLAG(FLAG_NEGATIVE | FLAG_HALFCARRY);

    return value;
}

uint8_t sra(Cpu* cpu, uint8_t value)
{
    if (value & 0x01)
        SET_FLAG(FLAG_CARRY);
    else
        CLEAR_FLAG(FLAG_CARRY);

    value = (value & 0x80) | (value >> 1);
    if (value == 0)
        SET_FLAG(FLAG_ZERO);
    else
        CLEAR_FLAG(FLAG_ZERO);

    CLEAR_FLAG(FLAG_NEGATIVE | FLAG_HALFCARRY);

    return value;
}

uint8_t swap(Cpu* cpu, uint8_t value)
{
    value = ((value & 0x0F) << 4) | ((value & 0xF0) >> 4);
    if (value == 0)
        SET_FLAG(FLAG_ZERO);
    else
        CLEAR_FLAG(FLAG_ZERO);

    CLEAR_FLAG(FLAG_NEGATIVE | FLAG_HALFCARRY | FLAG_CARRY);

    return value;
}

uint8_t srl(Cpu* cpu, uint8_t value)
{
    if (value & 0x01)
        SET_FLAG(FLAG_CARRY);
    else
        CLEAR_FLAG(FLAG_CARRY);

    value >>= 1;
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

void sla_b(Cpu* cpu, Memory* mem) { cpu->b = sla(cpu, cpu->b); }
void sla_c(Cpu* cpu, Memory* mem) { cpu->c = sla(cpu, cpu->c); }
void sla_d(Cpu* cpu, Memory* mem) { cpu->d = sla(cpu, cpu->d); }
void sla_e(Cpu* cpu, Memory* mem) { cpu->e = sla(cpu, cpu->e); }
void sla_h(Cpu* cpu, Memory* mem) { cpu->h = sla(cpu, cpu->h); }
void sla_l(Cpu* cpu, Memory* mem) { cpu->l = sla(cpu, cpu->l); }
void sla_at_hl(Cpu* cpu, Memory* mem) { memory_write(mem, get_hl(cpu), sla(cpu, memory_read(mem, get_hl(cpu)))); }
void sla_a(Cpu* cpu, Memory* mem) { cpu->a = sla(cpu, cpu->a); }

void sra_b(Cpu* cpu, Memory* mem) { cpu->b = sra(cpu, cpu->b); }
void sra_c(Cpu* cpu, Memory* mem) { cpu->c = sra(cpu, cpu->c); }
void sra_d(Cpu* cpu, Memory* mem) { cpu->d = sra(cpu, cpu->d); }
void sra_e(Cpu* cpu, Memory* mem) { cpu->e = sra(cpu, cpu->e); }
void sra_h(Cpu* cpu, Memory* mem) { cpu->h = sra(cpu, cpu->h); }
void sra_l(Cpu* cpu, Memory* mem) { cpu->l = sra(cpu, cpu->l); }
void sra_at_hl(Cpu* cpu, Memory* mem) { memory_write(mem, get_hl(cpu), sra(cpu, memory_read(mem, get_hl(cpu)))); }
void sra_a(Cpu* cpu, Memory* mem) { cpu->a = sra(cpu, cpu->a); }

void swap_b(Cpu* cpu, Memory* mem) { cpu->b = swap(cpu, cpu->b); }
void swap_c(Cpu* cpu, Memory* mem) { cpu->c = swap(cpu, cpu->c); }
void swap_d(Cpu* cpu, Memory* mem) { cpu->d = swap(cpu, cpu->d); }
void swap_e(Cpu* cpu, Memory* mem) { cpu->e = swap(cpu, cpu->e); }
void swap_h(Cpu* cpu, Memory* mem) { cpu->h = swap(cpu, cpu->h); }
void swap_l(Cpu* cpu, Memory* mem) { cpu->l = swap(cpu, cpu->l); }
void swap_at_hl(Cpu* cpu, Memory* mem) { memory_write(mem, get_hl(cpu), swap(cpu, memory_read(mem, get_hl(cpu)))); }
void swap_a(Cpu* cpu, Memory* mem) { cpu->a = swap(cpu, cpu->a); }

void srl_b(Cpu* cpu, Memory* mem) { cpu->b = srl(cpu, cpu->b); }
void srl_c(Cpu* cpu, Memory* mem) { cpu->c = srl(cpu, cpu->c); }
void srl_d(Cpu* cpu, Memory* mem) { cpu->d = srl(cpu, cpu->d); }
void srl_e(Cpu* cpu, Memory* mem) { cpu->e = srl(cpu, cpu->e); }
void srl_h(Cpu* cpu, Memory* mem) { cpu->h = srl(cpu, cpu->h); }
void srl_l(Cpu* cpu, Memory* mem) { cpu->l = srl(cpu, cpu->l); }
void srl_at_hl(Cpu* cpu, Memory* mem) { memory_write(mem, get_hl(cpu), srl(cpu, memory_read(mem, get_hl(cpu)))); }
void srl_a(Cpu* cpu, Memory* mem) { cpu->a = srl(cpu, cpu->a); }

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
    { "SLA B",      8, sla_b },
    { "SLA C",      8, sla_c },
    { "SLA D",      8, sla_d },
    { "SLA E",      8, sla_e },
    { "SLA H",      8, sla_h },
    { "SLA L",      8, sla_l },
    { "SLA [HL]",  16, sla_at_hl },
    { "SLA A",      8, sla_a },
    { "SRA B",      8, sra_b },
    { "SRA C",      8, sra_c },
    { "SRA D",      8, sra_d },
    { "SRA E",      8, sra_e },
    { "SRA H",      8, sra_h },
    { "SRA L",      8, sra_l },
    { "SRA [HL]",  16, sra_at_hl },
    { "SRA A",      8, sra_a },
    { "SWAP B",     8, swap_b },
    { "SWAP C",     8, swap_c },
    { "SWAP D",     8, swap_d },
    { "SWAP E",     8, swap_e },
    { "SWAP H",     8, swap_h },
    { "SWAP L",     8, swap_l },
    { "SWAP [HL]", 16, swap_at_hl },
    { "SWAP A",     8, swap_a },
    { "SRL B",      8, srl_b },
    { "SRL C",      8, srl_c },
    { "SRL D",      8, srl_d },
    { "SRL E",      8, srl_e },
    { "SRL H",      8, srl_h },
    { "SRL L",      8, srl_l },
    { "SRL [HL]",  16, srl_at_hl },
    { "SRL A",      8, srl_a },
};

void execute_cb_instruction(Cpu* cpu, Memory* mem, uint8_t byte)
{
    if (byte < 0 || byte > 0x100)
        return;

    CBInstruction cb_instruction = cb_instructions[byte];
    cb_instruction.handle(cpu, mem);
    cpu_add_ticks(cpu, cb_instruction.ticks);
}