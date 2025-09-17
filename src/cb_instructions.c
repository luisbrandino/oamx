#include "../inc/cb_instructions.h"
#include "assert.h"

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

void bit(Cpu* cpu, uint8_t bit_index, uint8_t value)
{
    assert(bit_index < 8);
    if (value & (1 << bit_index))
        CLEAR_FLAG(FLAG_ZERO);
    else
        SET_FLAG(FLAG_ZERO);

    CLEAR_FLAG(FLAG_NEGATIVE);
    SET_FLAG(FLAG_HALFCARRY);
}

uint8_t res(Cpu* cpu, uint8_t bit_index, uint8_t value)
{
    assert(bit_index < 8);
    return value & ~(1 << bit_index);
}

uint8_t set(Cpu* cpu, uint8_t bit_index, uint8_t value)
{
    assert(bit_index < 8);
    return value | (1 << bit_index);
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

void bit_0_b(Cpu* cpu, Memory* mem) { bit(cpu, 0, cpu->b); }
void bit_0_c(Cpu* cpu, Memory* mem) { bit(cpu, 0, cpu->c); }
void bit_0_d(Cpu* cpu, Memory* mem) { bit(cpu, 0, cpu->d); }
void bit_0_e(Cpu* cpu, Memory* mem) { bit(cpu, 0, cpu->e); }
void bit_0_h(Cpu* cpu, Memory* mem) { bit(cpu, 0, cpu->h); }
void bit_0_l(Cpu* cpu, Memory* mem) { bit(cpu, 0, cpu->l); }
void bit_0_at_hl(Cpu* cpu, Memory* mem) { bit(cpu, 0, memory_read(mem, get_hl(cpu))); }
void bit_0_a(Cpu* cpu, Memory* mem) { bit(cpu, 0, cpu->a); }

void bit_1_b(Cpu* cpu, Memory* mem) { bit(cpu, 1, cpu->b); }
void bit_1_c(Cpu* cpu, Memory* mem) { bit(cpu, 1, cpu->c); }
void bit_1_d(Cpu* cpu, Memory* mem) { bit(cpu, 1, cpu->d); }
void bit_1_e(Cpu* cpu, Memory* mem) { bit(cpu, 1, cpu->e); }
void bit_1_h(Cpu* cpu, Memory* mem) { bit(cpu, 1, cpu->h); }
void bit_1_l(Cpu* cpu, Memory* mem) { bit(cpu, 1, cpu->l); }
void bit_1_at_hl(Cpu* cpu, Memory* mem) { bit(cpu, 1, memory_read(mem, get_hl(cpu))); }
void bit_1_a(Cpu* cpu, Memory* mem) { bit(cpu, 1, cpu->a); }

void bit_2_b(Cpu* cpu, Memory* mem) { bit(cpu, 2, cpu->b); }
void bit_2_c(Cpu* cpu, Memory* mem) { bit(cpu, 2, cpu->c); }
void bit_2_d(Cpu* cpu, Memory* mem) { bit(cpu, 2, cpu->d); }
void bit_2_e(Cpu* cpu, Memory* mem) { bit(cpu, 2, cpu->e); }
void bit_2_h(Cpu* cpu, Memory* mem) { bit(cpu, 2, cpu->h); }
void bit_2_l(Cpu* cpu, Memory* mem) { bit(cpu, 2, cpu->l); }
void bit_2_at_hl(Cpu* cpu, Memory* mem) { bit(cpu, 2, memory_read(mem, get_hl(cpu))); }
void bit_2_a(Cpu* cpu, Memory* mem) { bit(cpu, 2, cpu->a); }

void bit_3_b(Cpu* cpu, Memory* mem) { bit(cpu, 3, cpu->b); }
void bit_3_c(Cpu* cpu, Memory* mem) { bit(cpu, 3, cpu->c); }
void bit_3_d(Cpu* cpu, Memory* mem) { bit(cpu, 3, cpu->d); }
void bit_3_e(Cpu* cpu, Memory* mem) { bit(cpu, 3, cpu->e); }
void bit_3_h(Cpu* cpu, Memory* mem) { bit(cpu, 3, cpu->h); }
void bit_3_l(Cpu* cpu, Memory* mem) { bit(cpu, 3, cpu->l); }
void bit_3_at_hl(Cpu* cpu, Memory* mem) { bit(cpu, 3, memory_read(mem, get_hl(cpu))); }
void bit_3_a(Cpu* cpu, Memory* mem) { bit(cpu, 3, cpu->a); }

void bit_4_b(Cpu* cpu, Memory* mem) { bit(cpu, 4, cpu->b); }
void bit_4_c(Cpu* cpu, Memory* mem) { bit(cpu, 4, cpu->c); }
void bit_4_d(Cpu* cpu, Memory* mem) { bit(cpu, 4, cpu->d); }
void bit_4_e(Cpu* cpu, Memory* mem) { bit(cpu, 4, cpu->e); }
void bit_4_h(Cpu* cpu, Memory* mem) { bit(cpu, 4, cpu->h); }
void bit_4_l(Cpu* cpu, Memory* mem) { bit(cpu, 4, cpu->l); }
void bit_4_at_hl(Cpu* cpu, Memory* mem) { bit(cpu, 4, memory_read(mem, get_hl(cpu))); }
void bit_4_a(Cpu* cpu, Memory* mem) { bit(cpu, 4, cpu->a); }

void bit_5_b(Cpu* cpu, Memory* mem) { bit(cpu, 5, cpu->b); }
void bit_5_c(Cpu* cpu, Memory* mem) { bit(cpu, 5, cpu->c); }
void bit_5_d(Cpu* cpu, Memory* mem) { bit(cpu, 5, cpu->d); }
void bit_5_e(Cpu* cpu, Memory* mem) { bit(cpu, 5, cpu->e); }
void bit_5_h(Cpu* cpu, Memory* mem) { bit(cpu, 5, cpu->h); }
void bit_5_l(Cpu* cpu, Memory* mem) { bit(cpu, 5, cpu->l); }
void bit_5_at_hl(Cpu* cpu, Memory* mem) { bit(cpu, 5, memory_read(mem, get_hl(cpu))); }
void bit_5_a(Cpu* cpu, Memory* mem) { bit(cpu, 5, cpu->a); }

void bit_6_b(Cpu* cpu, Memory* mem) { bit(cpu, 6, cpu->b); }
void bit_6_c(Cpu* cpu, Memory* mem) { bit(cpu, 6, cpu->c); }
void bit_6_d(Cpu* cpu, Memory* mem) { bit(cpu, 6, cpu->d); }
void bit_6_e(Cpu* cpu, Memory* mem) { bit(cpu, 6, cpu->e); }
void bit_6_h(Cpu* cpu, Memory* mem) { bit(cpu, 6, cpu->h); }
void bit_6_l(Cpu* cpu, Memory* mem) { bit(cpu, 6, cpu->l); }
void bit_6_at_hl(Cpu* cpu, Memory* mem) { bit(cpu, 6, memory_read(mem, get_hl(cpu))); }
void bit_6_a(Cpu* cpu, Memory* mem) { bit(cpu, 6, cpu->a); }

void bit_7_b(Cpu* cpu, Memory* mem) { bit(cpu, 7, cpu->b); }
void bit_7_c(Cpu* cpu, Memory* mem) { bit(cpu, 7, cpu->c); }
void bit_7_d(Cpu* cpu, Memory* mem) { bit(cpu, 7, cpu->d); }
void bit_7_e(Cpu* cpu, Memory* mem) { bit(cpu, 7, cpu->e); }
void bit_7_h(Cpu* cpu, Memory* mem) { bit(cpu, 7, cpu->h); }
void bit_7_l(Cpu* cpu, Memory* mem) { bit(cpu, 7, cpu->l); }
void bit_7_at_hl(Cpu* cpu, Memory* mem) { bit(cpu, 7, memory_read(mem, get_hl(cpu))); }
void bit_7_a(Cpu* cpu, Memory* mem) { bit(cpu, 7, cpu->a); }

void res_0_b(Cpu* cpu, Memory* mem) { cpu->b = res(cpu, 0, cpu->b); }
void res_0_c(Cpu* cpu, Memory* mem) { cpu->c = res(cpu, 0, cpu->c); }
void res_0_d(Cpu* cpu, Memory* mem) { cpu->d = res(cpu, 0, cpu->d); }
void res_0_e(Cpu* cpu, Memory* mem) { cpu->e = res(cpu, 0, cpu->e); }
void res_0_h(Cpu* cpu, Memory* mem) { cpu->h = res(cpu, 0, cpu->h); }
void res_0_l(Cpu* cpu, Memory* mem) { cpu->l = res(cpu, 0, cpu->l); }
void res_0_at_hl(Cpu* cpu, Memory* mem) { memory_write(mem, get_hl(cpu), res(cpu, 0, memory_read(mem, get_hl(cpu)))); }
void res_0_a(Cpu* cpu, Memory* mem) { cpu->a = res(cpu, 0, cpu->a); }

void res_1_b(Cpu* cpu, Memory* mem) { cpu->b = res(cpu, 1, cpu->b); }
void res_1_c(Cpu* cpu, Memory* mem) { cpu->c = res(cpu, 1, cpu->c); }
void res_1_d(Cpu* cpu, Memory* mem) { cpu->d = res(cpu, 1, cpu->d); }
void res_1_e(Cpu* cpu, Memory* mem) { cpu->e = res(cpu, 1, cpu->e); }
void res_1_h(Cpu* cpu, Memory* mem) { cpu->h = res(cpu, 1, cpu->h); }
void res_1_l(Cpu* cpu, Memory* mem) { cpu->l = res(cpu, 1, cpu->l); }
void res_1_at_hl(Cpu* cpu, Memory* mem) { memory_write(mem, get_hl(cpu), res(cpu, 1, memory_read(mem, get_hl(cpu)))); }
void res_1_a(Cpu* cpu, Memory* mem) { cpu->a = res(cpu, 1, cpu->a); }

void res_2_b(Cpu* cpu, Memory* mem) { cpu->b = res(cpu, 2, cpu->b); }
void res_2_c(Cpu* cpu, Memory* mem) { cpu->c = res(cpu, 2, cpu->c); }
void res_2_d(Cpu* cpu, Memory* mem) { cpu->d = res(cpu, 2, cpu->d); }
void res_2_e(Cpu* cpu, Memory* mem) { cpu->e = res(cpu, 2, cpu->e); }
void res_2_h(Cpu* cpu, Memory* mem) { cpu->h = res(cpu, 2, cpu->h); }
void res_2_l(Cpu* cpu, Memory* mem) { cpu->l = res(cpu, 2, cpu->l); }
void res_2_at_hl(Cpu* cpu, Memory* mem) { memory_write(mem, get_hl(cpu), res(cpu, 2, memory_read(mem, get_hl(cpu)))); }
void res_2_a(Cpu* cpu, Memory* mem) { cpu->a = res(cpu, 2, cpu->a); }

void res_3_b(Cpu* cpu, Memory* mem) { cpu->b = res(cpu, 3, cpu->b); }
void res_3_c(Cpu* cpu, Memory* mem) { cpu->c = res(cpu, 3, cpu->c); }
void res_3_d(Cpu* cpu, Memory* mem) { cpu->d = res(cpu, 3, cpu->d); }
void res_3_e(Cpu* cpu, Memory* mem) { cpu->e = res(cpu, 3, cpu->e); }
void res_3_h(Cpu* cpu, Memory* mem) { cpu->h = res(cpu, 3, cpu->h); }
void res_3_l(Cpu* cpu, Memory* mem) { cpu->l = res(cpu, 3, cpu->l); }
void res_3_at_hl(Cpu* cpu, Memory* mem) { memory_write(mem, get_hl(cpu), res(cpu, 3, memory_read(mem, get_hl(cpu)))); }
void res_3_a(Cpu* cpu, Memory* mem) { cpu->a = res(cpu, 3, cpu->a); }

void res_4_b(Cpu* cpu, Memory* mem) { cpu->b = res(cpu, 4, cpu->b); }
void res_4_c(Cpu* cpu, Memory* mem) { cpu->c = res(cpu, 4, cpu->c); }
void res_4_d(Cpu* cpu, Memory* mem) { cpu->d = res(cpu, 4, cpu->d); }
void res_4_e(Cpu* cpu, Memory* mem) { cpu->e = res(cpu, 4, cpu->e); }
void res_4_h(Cpu* cpu, Memory* mem) { cpu->h = res(cpu, 4, cpu->h); }
void res_4_l(Cpu* cpu, Memory* mem) { cpu->l = res(cpu, 4, cpu->l); }
void res_4_at_hl(Cpu* cpu, Memory* mem) { memory_write(mem, get_hl(cpu), res(cpu, 4, memory_read(mem, get_hl(cpu)))); }
void res_4_a(Cpu* cpu, Memory* mem) { cpu->a = res(cpu, 4, cpu->a); }

void res_5_b(Cpu* cpu, Memory* mem) { cpu->b = res(cpu, 5, cpu->b); }
void res_5_c(Cpu* cpu, Memory* mem) { cpu->c = res(cpu, 5, cpu->c); }
void res_5_d(Cpu* cpu, Memory* mem) { cpu->d = res(cpu, 5, cpu->d); }
void res_5_e(Cpu* cpu, Memory* mem) { cpu->e = res(cpu, 5, cpu->e); }
void res_5_h(Cpu* cpu, Memory* mem) { cpu->h = res(cpu, 5, cpu->h); }
void res_5_l(Cpu* cpu, Memory* mem) { cpu->l = res(cpu, 5, cpu->l); }
void res_5_at_hl(Cpu* cpu, Memory* mem) { memory_write(mem, get_hl(cpu), res(cpu, 5, memory_read(mem, get_hl(cpu)))); }
void res_5_a(Cpu* cpu, Memory* mem) { cpu->a = res(cpu, 5, cpu->a); }

void res_6_b(Cpu* cpu, Memory* mem) { cpu->b = res(cpu, 6, cpu->b); }
void res_6_c(Cpu* cpu, Memory* mem) { cpu->c = res(cpu, 6, cpu->c); }
void res_6_d(Cpu* cpu, Memory* mem) { cpu->d = res(cpu, 6, cpu->d); }
void res_6_e(Cpu* cpu, Memory* mem) { cpu->e = res(cpu, 6, cpu->e); }
void res_6_h(Cpu* cpu, Memory* mem) { cpu->h = res(cpu, 6, cpu->h); }
void res_6_l(Cpu* cpu, Memory* mem) { cpu->l = res(cpu, 6, cpu->l); }
void res_6_at_hl(Cpu* cpu, Memory* mem) { memory_write(mem, get_hl(cpu), res(cpu, 6, memory_read(mem, get_hl(cpu)))); }
void res_6_a(Cpu* cpu, Memory* mem) { cpu->a = res(cpu, 6, cpu->a); }

void res_7_b(Cpu* cpu, Memory* mem) { cpu->b = res(cpu, 7, cpu->b); }
void res_7_c(Cpu* cpu, Memory* mem) { cpu->c = res(cpu, 7, cpu->c); }
void res_7_d(Cpu* cpu, Memory* mem) { cpu->d = res(cpu, 7, cpu->d); }
void res_7_e(Cpu* cpu, Memory* mem) { cpu->e = res(cpu, 7, cpu->e); }
void res_7_h(Cpu* cpu, Memory* mem) { cpu->h = res(cpu, 7, cpu->h); }
void res_7_l(Cpu* cpu, Memory* mem) { cpu->l = res(cpu, 7, cpu->l); }
void res_7_at_hl(Cpu* cpu, Memory* mem) { memory_write(mem, get_hl(cpu), res(cpu, 7, memory_read(mem, get_hl(cpu)))); }
void res_7_a(Cpu* cpu, Memory* mem) { cpu->a = res(cpu, 7, cpu->a); }

void set_0_b(Cpu* cpu, Memory* mem) { cpu->b = set(cpu, 0, cpu->b); }
void set_0_c(Cpu* cpu, Memory* mem) { cpu->c = set(cpu, 0, cpu->c); }
void set_0_d(Cpu* cpu, Memory* mem) { cpu->d = set(cpu, 0, cpu->d); }
void set_0_e(Cpu* cpu, Memory* mem) { cpu->e = set(cpu, 0, cpu->e); }
void set_0_h(Cpu* cpu, Memory* mem) { cpu->h = set(cpu, 0, cpu->h); }
void set_0_l(Cpu* cpu, Memory* mem) { cpu->l = set(cpu, 0, cpu->l); }
void set_0_at_hl(Cpu* cpu, Memory* mem) { memory_write(mem, get_hl(cpu), set(cpu, 0, memory_read(mem, get_hl(cpu)))); }
void set_0_a(Cpu* cpu, Memory* mem) { cpu->a = set(cpu, 0, cpu->a); }

void set_1_b(Cpu* cpu, Memory* mem) { cpu->b = set(cpu, 1, cpu->b); }
void set_1_c(Cpu* cpu, Memory* mem) { cpu->c = set(cpu, 1, cpu->c); }
void set_1_d(Cpu* cpu, Memory* mem) { cpu->d = set(cpu, 1, cpu->d); }
void set_1_e(Cpu* cpu, Memory* mem) { cpu->e = set(cpu, 1, cpu->e); }
void set_1_h(Cpu* cpu, Memory* mem) { cpu->h = set(cpu, 1, cpu->h); }
void set_1_l(Cpu* cpu, Memory* mem) { cpu->l = set(cpu, 1, cpu->l); }
void set_1_at_hl(Cpu* cpu, Memory* mem) { memory_write(mem, get_hl(cpu), set(cpu, 1, memory_read(mem, get_hl(cpu)))); }
void set_1_a(Cpu* cpu, Memory* mem) { cpu->a = set(cpu, 1, cpu->a); }

void set_2_b(Cpu* cpu, Memory* mem) { cpu->b = set(cpu, 2, cpu->b); }
void set_2_c(Cpu* cpu, Memory* mem) { cpu->c = set(cpu, 2, cpu->c); }
void set_2_d(Cpu* cpu, Memory* mem) { cpu->d = set(cpu, 2, cpu->d); }
void set_2_e(Cpu* cpu, Memory* mem) { cpu->e = set(cpu, 2, cpu->e); }
void set_2_h(Cpu* cpu, Memory* mem) { cpu->h = set(cpu, 2, cpu->h); }
void set_2_l(Cpu* cpu, Memory* mem) { cpu->l = set(cpu, 2, cpu->l); }
void set_2_at_hl(Cpu* cpu, Memory* mem) { memory_write(mem, get_hl(cpu), set(cpu, 2, memory_read(mem, get_hl(cpu)))); }
void set_2_a(Cpu* cpu, Memory* mem) { cpu->a = set(cpu, 2, cpu->a); }

void set_3_b(Cpu* cpu, Memory* mem) { cpu->b = set(cpu, 3, cpu->b); }
void set_3_c(Cpu* cpu, Memory* mem) { cpu->c = set(cpu, 3, cpu->c); }
void set_3_d(Cpu* cpu, Memory* mem) { cpu->d = set(cpu, 3, cpu->d); }
void set_3_e(Cpu* cpu, Memory* mem) { cpu->e = set(cpu, 3, cpu->e); }
void set_3_h(Cpu* cpu, Memory* mem) { cpu->h = set(cpu, 3, cpu->h); }
void set_3_l(Cpu* cpu, Memory* mem) { cpu->l = set(cpu, 3, cpu->l); }
void set_3_at_hl(Cpu* cpu, Memory* mem) { memory_write(mem, get_hl(cpu), set(cpu, 3, memory_read(mem, get_hl(cpu)))); }
void set_3_a(Cpu* cpu, Memory* mem) { cpu->a = set(cpu, 3, cpu->a); }

void set_4_b(Cpu* cpu, Memory* mem) { cpu->b = set(cpu, 4, cpu->b); }
void set_4_c(Cpu* cpu, Memory* mem) { cpu->c = set(cpu, 4, cpu->c); }
void set_4_d(Cpu* cpu, Memory* mem) { cpu->d = set(cpu, 4, cpu->d); }
void set_4_e(Cpu* cpu, Memory* mem) { cpu->e = set(cpu, 4, cpu->e); }
void set_4_h(Cpu* cpu, Memory* mem) { cpu->h = set(cpu, 4, cpu->h); }
void set_4_l(Cpu* cpu, Memory* mem) { cpu->l = set(cpu, 4, cpu->l); }
void set_4_at_hl(Cpu* cpu, Memory* mem) { memory_write(mem, get_hl(cpu), set(cpu, 4, memory_read(mem, get_hl(cpu)))); }
void set_4_a(Cpu* cpu, Memory* mem) { cpu->a = set(cpu, 4, cpu->a); }

void set_5_b(Cpu* cpu, Memory* mem) { cpu->b = set(cpu, 5, cpu->b); }
void set_5_c(Cpu* cpu, Memory* mem) { cpu->c = set(cpu, 5, cpu->c); }
void set_5_d(Cpu* cpu, Memory* mem) { cpu->d = set(cpu, 5, cpu->d); }
void set_5_e(Cpu* cpu, Memory* mem) { cpu->e = set(cpu, 5, cpu->e); }
void set_5_h(Cpu* cpu, Memory* mem) { cpu->h = set(cpu, 5, cpu->h); }
void set_5_l(Cpu* cpu, Memory* mem) { cpu->l = set(cpu, 5, cpu->l); }
void set_5_at_hl(Cpu* cpu, Memory* mem) { memory_write(mem, get_hl(cpu), set(cpu, 5, memory_read(mem, get_hl(cpu)))); }
void set_5_a(Cpu* cpu, Memory* mem) { cpu->a = set(cpu, 5, cpu->a); }

void set_6_b(Cpu* cpu, Memory* mem) { cpu->b = set(cpu, 6, cpu->b); }
void set_6_c(Cpu* cpu, Memory* mem) { cpu->c = set(cpu, 6, cpu->c); }
void set_6_d(Cpu* cpu, Memory* mem) { cpu->d = set(cpu, 6, cpu->d); }
void set_6_e(Cpu* cpu, Memory* mem) { cpu->e = set(cpu, 6, cpu->e); }
void set_6_h(Cpu* cpu, Memory* mem) { cpu->h = set(cpu, 6, cpu->h); }
void set_6_l(Cpu* cpu, Memory* mem) { cpu->l = set(cpu, 6, cpu->l); }
void set_6_at_hl(Cpu* cpu, Memory* mem) { memory_write(mem, get_hl(cpu), set(cpu, 6, memory_read(mem, get_hl(cpu)))); }
void set_6_a(Cpu* cpu, Memory* mem) { cpu->a = set(cpu, 6, cpu->a); }

void set_7_b(Cpu* cpu, Memory* mem) { cpu->b = set(cpu, 7, cpu->b); }
void set_7_c(Cpu* cpu, Memory* mem) { cpu->c = set(cpu, 7, cpu->c); }
void set_7_d(Cpu* cpu, Memory* mem) { cpu->d = set(cpu, 7, cpu->d); }
void set_7_e(Cpu* cpu, Memory* mem) { cpu->e = set(cpu, 7, cpu->e); }
void set_7_h(Cpu* cpu, Memory* mem) { cpu->h = set(cpu, 7, cpu->h); }
void set_7_l(Cpu* cpu, Memory* mem) { cpu->l = set(cpu, 7, cpu->l); }
void set_7_at_hl(Cpu* cpu, Memory* mem) { memory_write(mem, get_hl(cpu), set(cpu, 7, memory_read(mem, get_hl(cpu)))); }
void set_7_a(Cpu* cpu, Memory* mem) { cpu->a = set(cpu, 7, cpu->a); }

const CBInstruction cb_instructions[0x100] = {
    { "RLC B",        8, rlc_b },
    { "RLC C",        8, rlc_c },
    { "RLC D",        8, rlc_d },
    { "RLC E",        8, rlc_e },
    { "RLC H",        8, rlc_h },
    { "RLC L",        8, rlc_l },
    { "RLC [HL]",    16, rlc_at_hl },
    { "RLC A",        8, rlc_a },
    { "RRC B",        8, rrc_b },
    { "RRC C",        8, rrc_c },
    { "RRC D",        8, rrc_d },
    { "RRC E",        8, rrc_e },
    { "RRC H",        8, rrc_h },
    { "RRC L",        8, rrc_l },
    { "RRC [HL]",    16, rrc_at_hl },
    { "RRC A",        8, rrc_a },
    { "RL B",         8, rl_b },
    { "RL C",         8, rl_c },
    { "RL D",         8, rl_d },
    { "RL E",         8, rl_e },
    { "RL H",         8, rl_h },
    { "RL L",         8, rl_l },
    { "RL [HL]",     16, rl_at_hl },
    { "RL A",         8, rl_a },
    { "RR B",         8, rr_b },
    { "RR C",         8, rr_c },
    { "RR D",         8, rr_d },
    { "RR E",         8, rr_e },
    { "RR H",         8, rr_h },
    { "RR L",         8, rr_l },
    { "RR [HL]",     16, rr_at_hl },
    { "RR A",         8, rr_a },
    { "SLA B",        8, sla_b },
    { "SLA C",        8, sla_c },
    { "SLA D",        8, sla_d },
    { "SLA E",        8, sla_e },
    { "SLA H",        8, sla_h },
    { "SLA L",        8, sla_l },
    { "SLA [HL]",    16, sla_at_hl },
    { "SLA A",        8, sla_a },
    { "SRA B",        8, sra_b },
    { "SRA C",        8, sra_c },
    { "SRA D",        8, sra_d },
    { "SRA E",        8, sra_e },
    { "SRA H",        8, sra_h },
    { "SRA L",        8, sra_l },
    { "SRA [HL]",    16, sra_at_hl },
    { "SRA A",        8, sra_a },
    { "SWAP B",       8, swap_b },
    { "SWAP C",       8, swap_c },
    { "SWAP D",       8, swap_d },
    { "SWAP E",       8, swap_e },
    { "SWAP H",       8, swap_h },
    { "SWAP L",       8, swap_l },
    { "SWAP [HL]",   16, swap_at_hl },
    { "SWAP A",       8, swap_a },
    { "SRL B",        8, srl_b },
    { "SRL C",        8, srl_c },
    { "SRL D",        8, srl_d },
    { "SRL E",        8, srl_e },
    { "SRL H",        8, srl_h },
    { "SRL L",        8, srl_l },
    { "SRL [HL]",    16, srl_at_hl },
    { "SRL A",        8, srl_a },
    { "BIT 0, B",     8, bit_0_b },
    { "BIT 0, C",     8, bit_0_c },
    { "BIT 0, D",     8, bit_0_d },
    { "BIT 0, E",     8, bit_0_e },
    { "BIT 0, H",     8, bit_0_h },
    { "BIT 0, L",     8, bit_0_l },
    { "BIT 0, [HL]", 12, bit_0_at_hl },
    { "BIT 0, A",     8, bit_0_a },
    { "BIT 1, B",     8, bit_1_b },
    { "BIT 1, C",     8, bit_1_c },
    { "BIT 1, D",     8, bit_1_d },
    { "BIT 1, E",     8, bit_1_e },
    { "BIT 1, H",     8, bit_1_h },
    { "BIT 1, L",     8, bit_1_l },
    { "BIT 1, [HL]", 12, bit_1_at_hl },
    { "BIT 1, A",     8, bit_1_a },
    { "BIT 2, B",     8, bit_2_b },
    { "BIT 2, C",     8, bit_2_c },
    { "BIT 2, D",     8, bit_2_d },
    { "BIT 2, E",     8, bit_2_e },
    { "BIT 2, H",     8, bit_2_h },
    { "BIT 2, L",     8, bit_2_l },
    { "BIT 2, [HL]", 12, bit_2_at_hl },
    { "BIT 2, A",     8, bit_2_a },
    { "BIT 3, B",     8, bit_3_b },
    { "BIT 3, C",     8, bit_3_c },
    { "BIT 3, D",     8, bit_3_d },
    { "BIT 3, E",     8, bit_3_e },
    { "BIT 3, H",     8, bit_3_h },
    { "BIT 3, L",     8, bit_3_l },
    { "BIT 3, [HL]", 12, bit_3_at_hl },
    { "BIT 3, A",     8, bit_3_a },
    { "BIT 4, B",     8, bit_4_b },
    { "BIT 4, C",     8, bit_4_c },
    { "BIT 4, D",     8, bit_4_d },
    { "BIT 4, E",     8, bit_4_e },
    { "BIT 4, H",     8, bit_4_h },
    { "BIT 4, L",     8, bit_4_l },
    { "BIT 4, [HL]", 12, bit_4_at_hl },
    { "BIT 4, A",     8, bit_4_a },
    { "BIT 5, B",     8, bit_5_b },
    { "BIT 5, C",     8, bit_5_c },
    { "BIT 5, D",     8, bit_5_d },
    { "BIT 5, E",     8, bit_5_e },
    { "BIT 5, H",     8, bit_5_h },
    { "BIT 5, L",     8, bit_5_l },
    { "BIT 5, [HL]", 12, bit_5_at_hl },
    { "BIT 5, A",     8, bit_5_a },
    { "BIT 6, B",     8, bit_6_b },
    { "BIT 6, C",     8, bit_6_c },
    { "BIT 6, D",     8, bit_6_d },
    { "BIT 6, E",     8, bit_6_e },
    { "BIT 6, H",     8, bit_6_h },
    { "BIT 6, L",     8, bit_6_l },
    { "BIT 6, [HL]", 12, bit_6_at_hl },
    { "BIT 6, A",     8, bit_6_a },
    { "BIT 7, B",     8, bit_7_b },
    { "BIT 7, C",     8, bit_7_c },
    { "BIT 7, D",     8, bit_7_d },
    { "BIT 7, E",     8, bit_7_e },
    { "BIT 7, H",     8, bit_7_h },
    { "BIT 7, L",     8, bit_7_l },
    { "BIT 7, [HL]", 12, bit_7_at_hl },
    { "BIT 7, A",     8, bit_7_a },
    { "RES 0, B",     8, res_0_b },
    { "RES 0, C",     8, res_0_c },
    { "RES 0, D",     8, res_0_d },
    { "RES 0, E",     8, res_0_e },
    { "RES 0, H",     8, res_0_h },
    { "RES 0, L",     8, res_0_l },
    { "RES 0, [HL]", 16, res_0_at_hl },
    { "RES 0, A",     8, res_0_a },
    { "RES 1, B",     8, res_1_b },
    { "RES 1, C",     8, res_1_c },
    { "RES 1, D",     8, res_1_d },
    { "RES 1, E",     8, res_1_e },
    { "RES 1, H",     8, res_1_h },
    { "RES 1, L",     8, res_1_l },
    { "RES 1, [HL]", 16, res_1_at_hl },
    { "RES 1, A",     8, res_1_a },
    { "RES 2, B",     8, res_2_b },
    { "RES 2, C",     8, res_2_c },
    { "RES 2, D",     8, res_2_d },
    { "RES 2, E",     8, res_2_e },
    { "RES 2, H",     8, res_2_h },
    { "RES 2, L",     8, res_2_l },
    { "RES 2, [HL]", 16, res_2_at_hl },
    { "RES 2, A",     8, res_2_a },
    { "RES 3, B",     8, res_3_b },
    { "RES 3, C",     8, res_3_c },
    { "RES 3, D",     8, res_3_d },
    { "RES 3, E",     8, res_3_e },
    { "RES 3, H",     8, res_3_h },
    { "RES 3, L",     8, res_3_l },
    { "RES 3, [HL]", 16, res_3_at_hl },
    { "RES 3, A",     8, res_3_a },
    { "RES 4, B",     8, res_4_b },
    { "RES 4, C",     8, res_4_c },
    { "RES 4, D",     8, res_4_d },
    { "RES 4, E",     8, res_4_e },
    { "RES 4, H",     8, res_4_h },
    { "RES 4, L",     8, res_4_l },
    { "RES 4, [HL]", 16, res_4_at_hl },
    { "RES 4, A",     8, res_4_a },
    { "RES 5, B",     8, res_5_b },
    { "RES 5, C",     8, res_5_c },
    { "RES 5, D",     8, res_5_d },
    { "RES 5, E",     8, res_5_e },
    { "RES 5, H",     8, res_5_h },
    { "RES 5, L",     8, res_5_l },
    { "RES 5, [HL]", 16, res_5_at_hl },
    { "RES 5, A",     8, res_5_a },
    { "RES 6, B",     8, res_6_b },
    { "RES 6, C",     8, res_6_c },
    { "RES 6, D",     8, res_6_d },
    { "RES 6, E",     8, res_6_e },
    { "RES 6, H",     8, res_6_h },
    { "RES 6, L",     8, res_6_l },
    { "RES 6, [HL]", 16, res_6_at_hl },
    { "RES 6, A",     8, res_6_a },
    { "RES 7, B",     8, res_7_b },
    { "RES 7, C",     8, res_7_c },
    { "RES 7, D",     8, res_7_d },
    { "RES 7, E",     8, res_7_e },
    { "RES 7, H",     8, res_7_h },
    { "RES 7, L",     8, res_7_l },
    { "RES 7, [HL]", 16, res_7_at_hl },
    { "RES 7, A",     8, res_7_a },
    { "SET 0, B",     8, set_0_b },
    { "SET 0, C",     8, set_0_c },
    { "SET 0, D",     8, set_0_d },
    { "SET 0, E",     8, set_0_e },
    { "SET 0, H",     8, set_0_h },
    { "SET 0, L",     8, set_0_l },
    { "SET 0, [HL]", 16, set_0_at_hl },
    { "SET 0, A",     8, set_0_a },
    { "SET 1, B",     8, set_1_b },
    { "SET 1, C",     8, set_1_c },
    { "SET 1, D",     8, set_1_d },
    { "SET 1, E",     8, set_1_e },
    { "SET 1, H",     8, set_1_h },
    { "SET 1, L",     8, set_1_l },
    { "SET 1, [HL]", 16, set_1_at_hl },
    { "SET 1, A",     8, set_1_a },
    { "SET 2, B",     8, set_2_b },
    { "SET 2, C",     8, set_2_c },
    { "SET 2, D",     8, set_2_d },
    { "SET 2, E",     8, set_2_e },
    { "SET 2, H",     8, set_2_h },
    { "SET 2, L",     8, set_2_l },
    { "SET 2, [HL]", 16, set_2_at_hl },
    { "SET 2, A",     8, set_2_a },
    { "SET 3, B",     8, set_3_b },
    { "SET 3, C",     8, set_3_c },
    { "SET 3, D",     8, set_3_d },
    { "SET 3, E",     8, set_3_e },
    { "SET 3, H",     8, set_3_h },
    { "SET 3, L",     8, set_3_l },
    { "SET 3, [HL]", 16, set_3_at_hl },
    { "SET 3, A",     8, set_3_a },
    { "SET 4, B",     8, set_4_b },
    { "SET 4, C",     8, set_4_c },
    { "SET 4, D",     8, set_4_d },
    { "SET 4, E",     8, set_4_e },
    { "SET 4, H",     8, set_4_h },
    { "SET 4, L",     8, set_4_l },
    { "SET 4, [HL]", 16, set_4_at_hl },
    { "SET 4, A",     8, set_4_a },
    { "SET 5, B",     8, set_5_b },
    { "SET 5, C",     8, set_5_c },
    { "SET 5, D",     8, set_5_d },
    { "SET 5, E",     8, set_5_e },
    { "SET 5, H",     8, set_5_h },
    { "SET 5, L",     8, set_5_l },
    { "SET 5, [HL]", 16, set_5_at_hl },
    { "SET 5, A",     8, set_5_a },
    { "SET 6, B",     8, set_6_b },
    { "SET 6, C",     8, set_6_c },
    { "SET 6, D",     8, set_6_d },
    { "SET 6, E",     8, set_6_e },
    { "SET 6, H",     8, set_6_h },
    { "SET 6, L",     8, set_6_l },
    { "SET 6, [HL]", 16, set_6_at_hl },
    { "SET 6, A",     8, set_6_a },
    { "SET 7, B",     8, set_7_b },
    { "SET 7, C",     8, set_7_c },
    { "SET 7, D",     8, set_7_d },
    { "SET 7, E",     8, set_7_e },
    { "SET 7, H",     8, set_7_h },
    { "SET 7, L",     8, set_7_l },
    { "SET 7, [HL]", 16, set_7_at_hl },
    { "SET 7, A",     8, set_7_a },
};

void execute_cb_instruction(Cpu* cpu, Memory* mem, uint8_t byte)
{
    if (byte < 0 || byte > 0x100)
        return;

    CBInstruction cb_instruction = cb_instructions[byte];
    cb_instruction.handle(cpu, mem);
    cpu_add_ticks(cpu, cb_instruction.ticks);
}