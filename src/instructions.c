#include "../inc/instructions.h"
#include "../inc/cb_instructions.h"

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

uint8_t add(Cpu* cpu, uint8_t src, uint8_t value)
{
    uint16_t result = src + value;

    if ((src & 0x0F) + (value & 0x0F) > 0x0F)
        SET_FLAG(FLAG_HALFCARRY);
    else
        CLEAR_FLAG(FLAG_HALFCARRY);

    if (result > 0xFF)
        SET_FLAG(FLAG_CARRY);
    else
        CLEAR_FLAG(FLAG_CARRY);

    src = (uint8_t)(result & 0xFF);

    if (src == 0)
        SET_FLAG(FLAG_ZERO);
    else
        CLEAR_FLAG(FLAG_ZERO);

    CLEAR_FLAG(FLAG_NEGATIVE);

    return src;
}

uint16_t add16(Cpu* cpu, uint16_t src, uint16_t value)
{
    uint32_t result = (uint32_t)src + (uint32_t)value;

    if (result & 0xFFFF0000)
        SET_FLAG(FLAG_CARRY);
    else
        CLEAR_FLAG(FLAG_CARRY);

    if ((src & 0x0FFF) + (value & 0x0FFF) > 0x0FFF)
        SET_FLAG(FLAG_HALFCARRY);
    else
        CLEAR_FLAG(FLAG_HALFCARRY);

    CLEAR_FLAG(FLAG_NEGATIVE);

    return (uint16_t)result;
}

uint8_t adc(Cpu* cpu, uint8_t src, uint8_t value)
{
    uint8_t carry_in = IS_FLAG_SET(FLAG_CARRY);
    uint16_t result = src + value + carry_in;

    if (((src & 0x0F) + (value & 0x0F) + carry_in) > 0x0F)
        SET_FLAG(FLAG_HALFCARRY);
    else
        CLEAR_FLAG(FLAG_HALFCARRY);

    if (result > 0xFF)
        SET_FLAG(FLAG_CARRY);
    else
        CLEAR_FLAG(FLAG_CARRY);

    src = (uint8_t)(result & 0xFF);

    if (src == 0)
        SET_FLAG(FLAG_ZERO);
    else
        CLEAR_FLAG(FLAG_ZERO);

    CLEAR_FLAG(FLAG_NEGATIVE);

    return src;
}

uint8_t sub(Cpu* cpu, uint8_t src, uint8_t value)
{
    uint8_t result = src - value;

    if ((src & 0x0F) < (value & 0x0F))
        SET_FLAG(FLAG_HALFCARRY);
    else
        CLEAR_FLAG(FLAG_HALFCARRY);

    if (src < value)
        SET_FLAG(FLAG_CARRY);
    else
        CLEAR_FLAG(FLAG_CARRY);
    
    if (result == 0)
        SET_FLAG(FLAG_ZERO);
    else
        CLEAR_FLAG(FLAG_ZERO);
    
    SET_FLAG(FLAG_NEGATIVE);

    return result;
}

uint8_t sbc(Cpu* cpu, uint8_t src, uint8_t value)
{
    uint8_t carry = IS_FLAG_SET(FLAG_CARRY);
    uint16_t result = (uint16_t)src - (uint16_t)value - carry;

    if (((src & 0x0F) - (value & 0x0F) - carry) & 0x10)
        SET_FLAG(FLAG_HALFCARRY);
    else
        CLEAR_FLAG(FLAG_HALFCARRY);

    if (result > 0xFF)
        SET_FLAG(FLAG_CARRY);
    else
        CLEAR_FLAG(FLAG_CARRY);

    if ((result & 0xFF) == 0)
        SET_FLAG(FLAG_ZERO);
    else
        CLEAR_FLAG(FLAG_ZERO);

    SET_FLAG(FLAG_NEGATIVE);

    return (uint8_t)result;
}

uint8_t and(Cpu* cpu, uint8_t src, uint8_t value)
{
    uint8_t result = src & value;

    if (result == 0)
        SET_FLAG(FLAG_ZERO);
    else
        CLEAR_FLAG(FLAG_ZERO);

    SET_FLAG(FLAG_HALFCARRY);
    CLEAR_FLAG(FLAG_NEGATIVE | FLAG_CARRY);

    return result;
}

uint8_t xor(Cpu* cpu, uint8_t src, uint8_t value)
{
    uint8_t result = src ^ value;

    if (result == 0)
        SET_FLAG(FLAG_ZERO);
    else
        CLEAR_FLAG(FLAG_ZERO);

    CLEAR_FLAG(FLAG_NEGATIVE | FLAG_HALFCARRY | FLAG_CARRY);

    return result;
}

uint8_t or(Cpu* cpu, uint8_t src, uint8_t value)
{
    uint8_t result = src | value;

    if (result == 0)
        SET_FLAG(FLAG_ZERO);
    else
        CLEAR_FLAG(FLAG_ZERO);

    CLEAR_FLAG(FLAG_NEGATIVE | FLAG_HALFCARRY | FLAG_CARRY);

    return result;
}

void cp(Cpu* cpu, uint8_t src, uint8_t value)
{
    if (src == value)
        SET_FLAG(FLAG_ZERO);
    else
        CLEAR_FLAG(FLAG_ZERO);

    if (src < value)
        SET_FLAG(FLAG_CARRY);
    else
        CLEAR_FLAG(FLAG_CARRY);

    if ((src & 0x0F) < (value & 0x0F))
        SET_FLAG(FLAG_HALFCARRY);
    else
        CLEAR_FLAG(FLAG_HALFCARRY);

    SET_FLAG(FLAG_NEGATIVE);
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
    cpu->b = instr->operand;
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
    cpu->c = instr->operand;
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

void stop(Instruction* instr, Cpu* cpu, Memory* mem)
{
    // this instruction should enter STOP mode, leaving the CPU in stand-by until a button is pressed.
    // in GBC, it is used to switch between double speed and normal speed CPU modes.

    cpu_advance_pc(cpu, OPERAND_BYTE);
    cpu->state = CPU_STOPPED;
}

void ld_de_nn(Instruction* instr, Cpu* cpu, Memory* mem)
{
    set_de(cpu, instr->operand16);
}

void ld_at_de_a(Instruction* instr, Cpu* cpu, Memory* mem)
{
    memory_write(mem, get_de(cpu), cpu->a);
}

void inc_de(Instruction* instr, Cpu* cpu, Memory* mem)
{
    set_de(cpu, get_de(cpu) + 1);
}

void inc_d(Instruction* instr, Cpu* cpu, Memory* mem)
{
    cpu->d = increment(cpu, cpu->d);
}

void dec_d(Instruction* instr, Cpu* cpu, Memory* mem)
{
    cpu->d = decrement(cpu, cpu->d);
}

void ld_d_n(Instruction* instr, Cpu* cpu, Memory* mem)
{
    cpu->d = instr->operand;
}

void rla(Instruction* instr, Cpu* cpu, Memory* mem)
{
    uint8_t carry_in = IS_FLAG_SET(FLAG_CARRY);
    uint8_t carry_out = (cpu->a >> 7) & 0x01;

    cpu->a = (cpu->a << 1) | carry_in;

    if (carry_out)
        SET_FLAG(FLAG_CARRY);
    else
        CLEAR_FLAG(FLAG_CARRY);
    
    CLEAR_FLAG(FLAG_NEGATIVE | FLAG_ZERO | FLAG_HALFCARRY);
}

void jr_e(Instruction* instr, Cpu* cpu, Memory* mem)
{
    cpu->pc += (int8_t)instr->operand + OPERAND_BYTE;
}

void add_hl_de(Instruction* instr, Cpu* cpu, Memory* mem)
{
    uint16_t hl = get_hl(cpu);
    uint16_t de = get_de(cpu);

    set_hl(cpu, add16(cpu, hl, de));
}

void ld_a_at_de(Instruction* instr, Cpu* cpu, Memory* mem)
{
    cpu->a = memory_read(mem, get_de(cpu));
}

void dec_de(Instruction* instr, Cpu* cpu, Memory* mem)
{
    set_de(cpu, get_de(cpu) - 1);
}

void inc_e(Instruction* instr, Cpu* cpu, Memory* mem)
{
    cpu->e = increment(cpu, cpu->e);
}

void dec_e(Instruction* instr, Cpu* cpu, Memory* mem)
{
    cpu->e = decrement(cpu, cpu->e);
}

void ld_e_n(Instruction* instr, Cpu* cpu, Memory* mem)
{
    cpu->e = instr->operand;
}

void rra(Instruction* instr, Cpu* cpu, Memory* mem)
{
    uint8_t carry_in = IS_FLAG_SET(FLAG_CARRY);
    uint8_t carry_out = cpu->a & 0x01;

    cpu->a = (cpu->a >> 1) | (carry_in << 7);

    if (carry_out)
        SET_FLAG(FLAG_CARRY);
    else
        CLEAR_FLAG(FLAG_CARRY);
    
    CLEAR_FLAG(FLAG_NEGATIVE | FLAG_ZERO | FLAG_HALFCARRY);
}

void jr_nz_e(Instruction* instr, Cpu* cpu, Memory* mem)
{
    cpu_advance_pc(cpu, OPERAND_BYTE);
    if (!IS_FLAG_SET(FLAG_ZERO))
    {
        cpu->pc += (int8_t)instr->operand;
        cpu_add_ticks(cpu, 4);
    }
}

void ld_hl_nn(Instruction* instr, Cpu* cpu, Memory* mem)
{
    set_hl(cpu, instr->operand16);
}

void ldi_at_hl_a(Instruction* instr, Cpu* cpu, Memory* mem)
{
    memory_write(mem, get_hl(cpu), cpu->a);
    set_hl(cpu, get_hl(cpu) + 1);
}

void inc_hl(Instruction* instr, Cpu* cpu, Memory* mem)
{
    set_hl(cpu, get_hl(cpu) + 1);
}

void inc_h(Instruction* instr, Cpu* cpu, Memory* mem)
{
    cpu->h = increment(cpu, cpu->h);
}

void dec_h(Instruction* instr, Cpu* cpu, Memory* mem)
{
    cpu->h = decrement(cpu, cpu->h);
}

void ld_h_n(Instruction* instr, Cpu* cpu, Memory* mem)
{
    cpu->h = instr->operand;
}

void daa(Instruction* instr, Cpu* cpu, Memory* mem)
{
    uint8_t adjustment = IS_FLAG_SET(FLAG_CARRY) ? 0x60 : 0x00;
    adjustment = IS_FLAG_SET(FLAG_HALFCARRY) ? adjustment | 0x06 : adjustment;

    if (!IS_FLAG_SET(FLAG_NEGATIVE))
    {
        if ((cpu->a & 0x0F) > 0x09) adjustment |= 0x06;
        if ((cpu->a > 0x99)) {
            adjustment |= 0x60;
            SET_FLAG(FLAG_CARRY);
        }

        cpu->a += adjustment;
    }
    else
    {
        cpu->a -= adjustment;
    }

    CLEAR_FLAG(FLAG_HALFCARRY);

    if (cpu->a == 0)
        SET_FLAG(FLAG_ZERO);
    else
        CLEAR_FLAG(FLAG_ZERO);
}

void jr_z_e(Instruction* instr, Cpu* cpu, Memory* mem)
{
    cpu_advance_pc(cpu, OPERAND_BYTE);
    if (IS_FLAG_SET(FLAG_ZERO))
    {
        cpu->pc += (int8_t)instr->operand;
        cpu_add_ticks(cpu, 4);
    }
}

void add_hl_hl(Instruction* instr, Cpu* cpu, Memory* mem)
{
    uint16_t hl = get_hl(cpu);
    set_hl(cpu, add16(cpu, hl, hl));
}

void ldi_a_at_hl(Instruction* instr, Cpu* cpu, Memory* mem)
{
    cpu->a = memory_read(mem, get_hl(cpu));
    set_hl(cpu, get_hl(cpu) + 1);
}

void dec_hl(Instruction* instr, Cpu* cpu, Memory* mem)
{
    set_hl(cpu, get_hl(cpu) - 1);
}

void inc_l(Instruction* instr, Cpu* cpu, Memory* mem)
{
    cpu->l = increment(cpu, cpu->l);
}

void dec_l(Instruction* instr, Cpu* cpu, Memory* mem)
{
    cpu->l = decrement(cpu, cpu->l);
}

void ld_l_n(Instruction* instr, Cpu* cpu, Memory* mem)
{
    cpu->l = instr->operand;
}

void cpl(Instruction* instr, Cpu* cpu, Memory* mem)
{
    cpu->a = ~cpu->a;
    SET_FLAG(FLAG_NEGATIVE | FLAG_HALFCARRY);
}

void jr_nc_e(Instruction* instr, Cpu* cpu, Memory* mem)
{
    cpu_advance_pc(cpu, OPERAND_BYTE);
    if (!IS_FLAG_SET(FLAG_CARRY))
    {
        cpu->pc += (int8_t)instr->operand;
        cpu_add_ticks(cpu, 4);
    }
}

void ld_sp_nn(Instruction* instr, Cpu* cpu, Memory* mem)
{
    cpu->sp = instr->operand16;
}

void ldd_at_hl_a(Instruction* instr, Cpu* cpu, Memory* mem)
{
    uint16_t hl = get_hl(cpu);
    memory_write(mem, hl, cpu->a);
    set_hl(cpu, hl - 1);
}

void inc_sp(Instruction* instr, Cpu* cpu, Memory* mem)
{
    cpu->sp++;
}

void inc_at_hl(Instruction* instr, Cpu* cpu, Memory* mem)
{
    uint16_t hl = get_hl(cpu);
    uint8_t value = memory_read(mem, hl);
    memory_write(mem, hl, increment(cpu, value));
}

void dec_at_hl(Instruction* instr, Cpu* cpu, Memory* mem)
{
    uint16_t hl = get_hl(cpu);
    uint8_t value = memory_read(mem, hl);
    memory_write(mem, hl, decrement(cpu, value));
}

void ld_at_hl_n(Instruction* instr, Cpu* cpu, Memory* mem)
{
    memory_write(mem, get_hl(cpu), instr->operand);
}

void scf(Instruction* instr, Cpu* cpu, Memory* mem)
{
    SET_FLAG(FLAG_CARRY);
    CLEAR_FLAG(FLAG_NEGATIVE | FLAG_HALFCARRY);
}

void jr_c_e(Instruction* instr, Cpu* cpu, Memory* mem)
{
    cpu_advance_pc(cpu, OPERAND_BYTE);
    if (IS_FLAG_SET(FLAG_CARRY))
    {
        cpu->pc += (int8_t)instr->operand;
        cpu_add_ticks(cpu, 4);
    }
}

void add_hl_sp(Instruction* instr, Cpu* cpu, Memory* mem)
{
    uint16_t hl = get_hl(cpu);
    set_hl(cpu, add16(cpu, hl, cpu->sp));
}

void ldd_a_at_hl(Instruction* instr, Cpu* cpu, Memory* mem)
{
    uint16_t hl = get_hl(cpu);
    cpu->a = memory_read(mem, hl);
    set_hl(cpu, hl - 1);
}

void dec_sp(Instruction* instr, Cpu* cpu, Memory* mem)
{
    cpu->sp--;
}

void inc_a(Instruction* instr, Cpu* cpu, Memory* mem)
{
    cpu->a = increment(cpu, cpu->a);
}

void dec_a(Instruction* instr, Cpu* cpu, Memory* mem)
{
    cpu->a = decrement(cpu, cpu->a);
}

void ld_a_n(Instruction* instr, Cpu* cpu, Memory* mem)
{
    cpu->a = instr->operand;
}

void ccf(Instruction* instr, Cpu* cpu, Memory* mem)
{
    if (IS_FLAG_SET(FLAG_CARRY))
        CLEAR_FLAG(FLAG_CARRY);
    else
        SET_FLAG(FLAG_CARRY);

    CLEAR_FLAG(FLAG_NEGATIVE | FLAG_HALFCARRY);
}

void ld_b_c(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->b = cpu->c; }

void ld_b_d(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->b = cpu->d; }

void ld_b_e(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->b = cpu->e; }

void ld_b_h(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->b = cpu->h; }

void ld_b_l(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->b = cpu->l; }

void ld_b_at_hl(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->b = memory_read(mem, get_hl(cpu)); }

void ld_b_a(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->b = cpu->a; }

void ld_c_b(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->c = cpu->b; }

void ld_c_d(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->c = cpu->d; }

void ld_c_e(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->c = cpu->e; }

void ld_c_h(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->c = cpu->h; }

void ld_c_l(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->c = cpu->l; }

void ld_c_at_hl(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->c = memory_read(mem, get_hl(cpu)); }

void ld_c_a(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->c = cpu->a; }

void ld_d_b(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->d = cpu->b; }

void ld_d_c(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->d = cpu->c; }

void ld_d_e(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->d = cpu->e; }

void ld_d_h(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->d = cpu->h; }

void ld_d_l(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->d = cpu->l; }

void ld_d_at_hl(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->d = memory_read(mem, get_hl(cpu)); }

void ld_d_a(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->d = cpu->a; }

void ld_e_b(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->e = cpu->b; }

void ld_e_c(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->e = cpu->c; }

void ld_e_d(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->e = cpu->d; }

void ld_e_h(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->e = cpu->h; }

void ld_e_l(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->e = cpu->l; }

void ld_e_at_hl(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->e = memory_read(mem, get_hl(cpu)); }

void ld_e_a(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->e = cpu->a; }

void ld_h_b(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->h = cpu->b; }

void ld_h_c(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->h = cpu->c; }

void ld_h_d(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->h = cpu->d; }

void ld_h_e(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->h = cpu->e; }

void ld_h_l(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->h = cpu->l; }

void ld_h_at_hl(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->h = memory_read(mem, get_hl(cpu)); }

void ld_h_a(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->h = cpu->a; }

void ld_l_b(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->l = cpu->b; }

void ld_l_c(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->l = cpu->c; }

void ld_l_d(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->l = cpu->d; }

void ld_l_e(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->l = cpu->e; }

void ld_l_h(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->l = cpu->h; }

void ld_l_at_hl(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->l = memory_read(mem, get_hl(cpu)); }

void ld_l_a(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->l = cpu->a; }

void ld_at_hl_b(Instruction* instr, Cpu* cpu, Memory* mem) { memory_write(mem, get_hl(cpu), cpu->b); }

void ld_at_hl_c(Instruction* instr, Cpu* cpu, Memory* mem) { memory_write(mem, get_hl(cpu), cpu->c); }

void ld_at_hl_d(Instruction* instr, Cpu* cpu, Memory* mem) { memory_write(mem, get_hl(cpu), cpu->d); }

void ld_at_hl_e(Instruction* instr, Cpu* cpu, Memory* mem) { memory_write(mem, get_hl(cpu), cpu->e); }

void ld_at_hl_h(Instruction* instr, Cpu* cpu, Memory* mem) { memory_write(mem, get_hl(cpu), cpu->h); }

void ld_at_hl_l(Instruction* instr, Cpu* cpu, Memory* mem) { memory_write(mem, get_hl(cpu), cpu->l); }

void halt(Instruction* instr, Cpu* cpu, Memory* mem)
{
    cpu->state = CPU_HALTED;
}

void ld_at_hl_a(Instruction* instr, Cpu* cpu, Memory* mem) { memory_write(mem, get_hl(cpu), cpu->a); }

void ld_a_b(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = cpu->b; }

void ld_a_c(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = cpu->c; }

void ld_a_d(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = cpu->d; }

void ld_a_e(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = cpu->e; }

void ld_a_h(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = cpu->h; }

void ld_a_l(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = cpu->l; }

void ld_a_at_hl(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = memory_read(mem, get_hl(cpu)); }

void add_a_b(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = add(cpu, cpu->a, cpu->b); }

void add_a_c(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = add(cpu, cpu->a, cpu->c); }

void add_a_d(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = add(cpu, cpu->a, cpu->d); }

void add_a_e(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = add(cpu, cpu->a, cpu->e); }

void add_a_h(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = add(cpu, cpu->a, cpu->h); }

void add_a_l(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = add(cpu, cpu->a, cpu->l); }

void add_a_at_hl(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = add(cpu, cpu->a, memory_read(mem, get_hl(cpu))); }

void add_a_a(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = add(cpu, cpu->a, cpu->a); }

void adc_a_b(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = adc(cpu, cpu->a, cpu->b); }

void adc_a_c(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = adc(cpu, cpu->a, cpu->c); }

void adc_a_d(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = adc(cpu, cpu->a, cpu->d); }

void adc_a_e(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = adc(cpu, cpu->a, cpu->e); }

void adc_a_h(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = adc(cpu, cpu->a, cpu->h); }

void adc_a_l(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = adc(cpu, cpu->a, cpu->l); }

void adc_a_at_hl(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = adc(cpu, cpu->a, memory_read(mem, get_hl(cpu))); }

void adc_a_a(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = adc(cpu, cpu->a, cpu->a); }

void sub_a_b(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = sub(cpu, cpu->a, cpu->b); }

void sub_a_c(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = sub(cpu, cpu->a, cpu->c); }

void sub_a_d(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = sub(cpu, cpu->a, cpu->d); }

void sub_a_e(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = sub(cpu, cpu->a, cpu->e); }

void sub_a_h(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = sub(cpu, cpu->a, cpu->h); }

void sub_a_l(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = sub(cpu, cpu->a, cpu->l); }

void sub_a_at_hl(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = sub(cpu, cpu->a, memory_read(mem, get_hl(cpu))); }

void sub_a_a(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = sub(cpu, cpu->a, cpu->a); }

void sbc_a_b(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = sbc(cpu, cpu->a, cpu->b); }

void sbc_a_c(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = sbc(cpu, cpu->a, cpu->c); }

void sbc_a_d(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = sbc(cpu, cpu->a, cpu->d); }

void sbc_a_e(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = sbc(cpu, cpu->a, cpu->e); }

void sbc_a_h(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = sbc(cpu, cpu->a, cpu->h); }

void sbc_a_l(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = sbc(cpu, cpu->a, cpu->l); }

void sbc_a_at_hl(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = sbc(cpu, cpu->a, memory_read(mem, get_hl(cpu))); }

void sbc_a_a(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = sbc(cpu, cpu->a, cpu->a); }

void and_a_b(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = and(cpu, cpu->a, cpu->b); }

void and_a_c(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = and(cpu, cpu->a, cpu->c); }

void and_a_d(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = and(cpu, cpu->a, cpu->d); }

void and_a_e(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = and(cpu, cpu->a, cpu->e); }

void and_a_h(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = and(cpu, cpu->a, cpu->h); }

void and_a_l(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = and(cpu, cpu->a, cpu->l); }

void and_a_at_hl(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = and(cpu, cpu->a, memory_read(mem, get_hl(cpu))); }

void and_a_a(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = and(cpu, cpu->a, cpu->a); }

void xor_a_b(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = xor(cpu, cpu->a, cpu->b); }

void xor_a_c(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = xor(cpu, cpu->a, cpu->c); }

void xor_a_d(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = xor(cpu, cpu->a, cpu->d); }

void xor_a_e(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = xor(cpu, cpu->a, cpu->e); }

void xor_a_h(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = xor(cpu, cpu->a, cpu->h); }

void xor_a_l(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = xor(cpu, cpu->a, cpu->l); }

void xor_a_at_hl(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = xor(cpu, cpu->a, memory_read(mem, get_hl(cpu))); }

void xor_a_a(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = xor(cpu, cpu->a, cpu->a); }

void or_a_b(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = or(cpu, cpu->a, cpu->b); }

void or_a_c(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = or(cpu, cpu->a, cpu->c); }

void or_a_d(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = or(cpu, cpu->a, cpu->d); }

void or_a_e(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = or(cpu, cpu->a, cpu->e); }

void or_a_h(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = or(cpu, cpu->a, cpu->h); }

void or_a_l(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = or(cpu, cpu->a, cpu->l); }

void or_a_at_hl(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = or(cpu, cpu->a, memory_read(mem, get_hl(cpu))); }

void or_a_a(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = or(cpu, cpu->a, cpu->a); }

void cp_a_b(Instruction* instr, Cpu* cpu, Memory* mem) { cp(cpu, cpu->a, cpu->b); }

void cp_a_c(Instruction* instr, Cpu* cpu, Memory* mem) { cp(cpu, cpu->a, cpu->c); }

void cp_a_d(Instruction* instr, Cpu* cpu, Memory* mem) { cp(cpu, cpu->a, cpu->d); }

void cp_a_e(Instruction* instr, Cpu* cpu, Memory* mem) { cp(cpu, cpu->a, cpu->e); }

void cp_a_h(Instruction* instr, Cpu* cpu, Memory* mem) { cp(cpu, cpu->a, cpu->h); }

void cp_a_l(Instruction* instr, Cpu* cpu, Memory* mem) { cp(cpu, cpu->a, cpu->l); }

void cp_a_at_hl(Instruction* instr, Cpu* cpu, Memory* mem) { cp(cpu, cpu->a, memory_read(mem, get_hl(cpu))); }

void cp_a_a(Instruction* instr, Cpu* cpu, Memory* mem) { cp(cpu, cpu->a, cpu->a); }

void ret_nz(Instruction* instr, Cpu* cpu, Memory* mem)
{
    if (!IS_FLAG_SET(FLAG_ZERO))
    {
        cpu_ret(cpu, mem);
        cpu_add_ticks(cpu, 12);
    }
}

void pop_bc(Instruction* instr, Cpu* cpu, Memory* mem) { set_bc(cpu, cpu_pop(cpu, mem)); }

void jp_nz_nn(Instruction* instr, Cpu* cpu, Memory* mem)
{
    if (!IS_FLAG_SET(FLAG_ZERO))
    {
        cpu->pc = instr->operand16;
        cpu_add_ticks(cpu, 4);
    } 
    else
    {
        cpu_advance_pc(cpu, OPERAND_WORD);
    }
}

void jp_nn(Instruction* instr, Cpu* cpu, Memory* mem)
{
    cpu->pc = instr->operand16;
}

void call_nz_nn(Instruction* instr, Cpu* cpu, Memory* mem)
{
    cpu_advance_pc(cpu, OPERAND_WORD);
    if (!IS_FLAG_SET(FLAG_ZERO))
    {
        cpu_call(cpu, mem, instr->operand16);
        cpu_add_ticks(cpu, 12);
    }
}

void push_bc(Instruction* instr, Cpu* cpu, Memory* mem) { cpu_push(cpu, mem, get_bc(cpu)); }

void add_a_n(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = add(cpu, cpu->a, instr->operand); }

void rst_00(Instruction* instr, Cpu* cpu, Memory* mem) { cpu_call(cpu, mem, 0x0000); }

void ret_z(Instruction* instr, Cpu* cpu, Memory* mem)
{
    if (IS_FLAG_SET(FLAG_ZERO))
    {
        cpu_ret(cpu, mem);
        cpu_add_ticks(cpu, 12);
    }
}

void ret(Instruction* instr, Cpu* cpu, Memory* mem) { cpu_ret(cpu, mem); }

void jp_z_nn(Instruction* instr, Cpu* cpu, Memory* mem)
{
    if (IS_FLAG_SET(FLAG_ZERO))
    {
        cpu->pc = instr->operand16;
        cpu_add_ticks(cpu, 4);
    }
    else
    {
        cpu_advance_pc(cpu, OPERAND_WORD);
    }
}

void cb_n(Instruction* instr, Cpu* cpu, Memory* mem)
{
    // instr->operand = next opcode
    // no need to advance PC

    execute_cb_instruction(cpu, mem, instr->operand);
}

void call_z_nn(Instruction* instr, Cpu* cpu, Memory* mem)
{
    cpu_advance_pc(cpu, OPERAND_WORD);
    if (IS_FLAG_SET(FLAG_ZERO))
    {
        cpu_call(cpu, mem, instr->operand16);
        cpu_add_ticks(cpu, 12);
    }
}

void call_nn(Instruction* instr, Cpu* cpu, Memory* mem)
{
    cpu_advance_pc(cpu, OPERAND_WORD);
    cpu_call(cpu, mem, instr->operand16);
}

void adc_a_n(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = adc(cpu, cpu->a, instr->operand); }

void rst_08(Instruction* instr, Cpu* cpu, Memory* mem) { cpu_call(cpu, mem, 0x0008); }

void ret_nc(Instruction* instr, Cpu* cpu, Memory* mem)
{
    if (!IS_FLAG_SET(FLAG_CARRY))
    {
        cpu_ret(cpu, mem);
        cpu_add_ticks(cpu, 12);
    }
}

void pop_de(Instruction* instr, Cpu* cpu, Memory* mem) { set_de(cpu, cpu_pop(cpu, mem)); }

void jp_nc_nn(Instruction* instr, Cpu* cpu, Memory* mem)
{
    if (!IS_FLAG_SET(FLAG_CARRY))
    {
        cpu->pc = instr->operand16;
        cpu_add_ticks(cpu, 4);
    }
    else
    {
        cpu_advance_pc(cpu, OPERAND_WORD);
    }
}

void call_nc_nn(Instruction* instr, Cpu* cpu, Memory* mem)
{
    cpu_advance_pc(cpu, OPERAND_WORD);
    if (!IS_FLAG_SET(FLAG_CARRY))
    {
        cpu_call(cpu, mem, instr->operand16);
        cpu_add_ticks(cpu, 12);
    }
}

void push_de(Instruction* instr, Cpu* cpu, Memory* mem) { cpu_push(cpu, mem, get_de(cpu)); }

void sub_a_n(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = sub(cpu, cpu->a, instr->operand); }

void rst_10(Instruction* instr, Cpu* cpu, Memory* mem) { cpu_call(cpu, mem, 0x0010); }

void ret_c(Instruction* instr, Cpu* cpu, Memory* mem)
{
    if (IS_FLAG_SET(FLAG_CARRY))
    {
        cpu_ret(cpu, mem);
        cpu_add_ticks(cpu, 12);
    }
}

void reti(Instruction* instr, Cpu* cpu, Memory* mem)
{
    cpu->ime = 1;
    cpu_ret(cpu, mem);
}

void jp_c_nn(Instruction* instr, Cpu* cpu, Memory* mem)
{
    if (IS_FLAG_SET(FLAG_CARRY))
    {
        cpu->pc = instr->operand16;
        cpu_add_ticks(cpu, 4);
    }
    else
    {
        cpu_advance_pc(cpu, OPERAND_WORD);
    }
}

void call_c_nn(Instruction* instr, Cpu* cpu, Memory* mem)
{
    cpu_advance_pc(cpu, OPERAND_WORD);
    if (IS_FLAG_SET(FLAG_CARRY))
    {
        cpu_call(cpu, mem, instr->operand16);
        cpu_add_ticks(cpu, 12);
    }
}

void sbc_a_n(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = sbc(cpu, cpu->a, instr->operand); }

void rst_18(Instruction* instr, Cpu* cpu, Memory* mem) { cpu_call(cpu, mem, 0x0018); }

void ldh_at_n_a(Instruction* instr, Cpu* cpu, Memory* mem) { memory_write(mem, 0xFF00 + instr->operand, cpu->a); }

void pop_hl(Instruction* instr, Cpu* cpu, Memory* mem) { set_hl(cpu, cpu_pop(cpu, mem)); }

void ldh_at_c_a(Instruction* instr, Cpu* cpu, Memory* mem) { memory_write(mem, 0xFF00 + cpu->c, cpu->a); }

void push_hl(Instruction* instr, Cpu* cpu, Memory* mem) { cpu_push(cpu, mem, get_hl(cpu)); }

void and_a_n(Instruction* instr, Cpu* cpu, Memory* mem)
{
    cpu->a &= instr->operand;

    CLEAR_FLAG(FLAG_NEGATIVE | FLAG_CARRY);
    SET_FLAG(FLAG_HALFCARRY);
    if (cpu->a)
        CLEAR_FLAG(FLAG_ZERO);
    else
        SET_FLAG(FLAG_ZERO);
}

void rst_20(Instruction* instr, Cpu* cpu, Memory* mem) { cpu_call(cpu, mem, 0x0020); }

void add_sp_n(Instruction* instr, Cpu* cpu, Memory* mem)
{
    int8_t operand = (int8_t)instr->operand;
    uint16_t result = cpu->sp + operand;
    if (((cpu->sp ^ operand) ^ result) & (1 << 4))
        SET_FLAG(FLAG_HALFCARRY);
    else
        CLEAR_FLAG(FLAG_HALFCARRY);
    
    if (((cpu->sp ^ operand) ^ result) & (1 << 8))
        SET_FLAG(FLAG_CARRY);
    else
        CLEAR_FLAG(FLAG_CARRY);

    CLEAR_FLAG(FLAG_ZERO | FLAG_NEGATIVE);

    cpu->sp = result;
}

void jp_hl(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->pc = get_hl(cpu); }

void ld_at_nn_a(Instruction* instr, Cpu* cpu, Memory* mem) { memory_write(mem, instr->operand16, cpu->a); }

void xor_a_n(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = xor(cpu, cpu->a, instr->operand); }

void rst_28(Instruction* instr, Cpu* cpu, Memory* mem) { cpu_call(cpu, mem, 0x0028); }

void ldh_a_at_n(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = memory_read(mem, 0xFF00 + instr->operand); }

void pop_af(Instruction* instr, Cpu* cpu, Memory* mem) { set_af(cpu, cpu_pop(cpu, mem)); }

void ldh_a_at_c(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = memory_read(mem, 0xFF00 + cpu->c); }

void di(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->ime = 0; }

void push_af(Instruction* instr, Cpu* cpu, Memory* mem) { cpu_push(cpu, mem, get_af(cpu)); }

void or_a_n(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = or(cpu, cpu->a, instr->operand); }

void rst_30(Instruction* instr, Cpu* cpu, Memory* mem) { cpu_call(cpu, mem, 0x0030); }

void ld_hl_sp_plus_n(Instruction* instr, Cpu* cpu, Memory* mem)
{
    int8_t operand = (int8_t)instr->operand;
    uint16_t result = cpu->sp + operand;
    if (((cpu->sp ^ operand ^ result)) & (1 << 4))
        SET_FLAG(FLAG_HALFCARRY);
    else
        CLEAR_FLAG(FLAG_HALFCARRY);

    if (((cpu->sp ^ operand ^ result)) & (1 << 8))
        SET_FLAG(FLAG_CARRY);
    else
        CLEAR_FLAG(FLAG_CARRY);

    CLEAR_FLAG(FLAG_ZERO | FLAG_NEGATIVE);

    set_hl(cpu, result);
}

void ld_sp_hl(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->sp = get_hl(cpu); }

void ld_a_at_nn(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->a = memory_read(mem, instr->operand16); }

void ei(Instruction* instr, Cpu* cpu, Memory* mem) { cpu->ime = 1; }

void cp_a_n(Instruction* instr, Cpu* cpu, Memory* mem) { cp(cpu, cpu->a, instr->operand); }

void rst_38(Instruction* instr, Cpu* cpu, Memory* mem) { cpu_call(cpu, mem, 0x0038); }

const Instruction instructions[0x100] = {
    { "NOP",            4,  OPERAND_NONE, PC_ADVANCE, .handle = nop },
    { "LD BC, nn",     12,  OPERAND_WORD, PC_ADVANCE, .handle = ld_bc_nn },
    { "LD [BC], A",     8,  OPERAND_NONE, PC_ADVANCE, .handle = ld_at_bc_a },
    { "INC BC",         8,  OPERAND_NONE, PC_ADVANCE, .handle = inc_bc },
    { "INC B",          4,  OPERAND_NONE, PC_ADVANCE, .handle = inc_b },
    { "DEC B",          4,  OPERAND_NONE, PC_ADVANCE, .handle = dec_b },
    { "LD B, n",        8,  OPERAND_BYTE, PC_ADVANCE, .handle = ld_b_n },
    { "RLCA",           4,  OPERAND_NONE, PC_ADVANCE, .handle = rlca },
    { "LD [nn], SP",   20,  OPERAND_WORD, PC_ADVANCE, .handle = ld_at_nn_sp },
    { "ADD HL, BC",     8,  OPERAND_NONE, PC_ADVANCE, .handle = add_hl_bc },
    { "LD A, [BC]",     8,  OPERAND_NONE, PC_ADVANCE, .handle = ld_a_at_bc },
    { "DEC BC",         8,  OPERAND_NONE, PC_ADVANCE, .handle = dec_bc },
    { "INC C",          4,  OPERAND_NONE, PC_ADVANCE, .handle = inc_c },
    { "DEC C",          4,  OPERAND_NONE, PC_ADVANCE, .handle = dec_c },
    { "LD C, n",        8,  OPERAND_BYTE, PC_ADVANCE, .handle = ld_c_n },
    { "RRCA",           4,  OPERAND_NONE, PC_ADVANCE, .handle = rrca },
    { "STOP n",         4,  OPERAND_BYTE, PC_MANUAL, .handle = stop },
    { "LD DE, nn",     12,  OPERAND_WORD, PC_ADVANCE, .handle = ld_de_nn },
    { "LD [DE], A",     8,  OPERAND_NONE, PC_ADVANCE, .handle = ld_at_de_a },
    { "INC DE",         8,  OPERAND_NONE, PC_ADVANCE, .handle = inc_de },
    { "INC D",          4,  OPERAND_NONE, PC_ADVANCE, .handle = inc_d },
    { "DEC D",          4,  OPERAND_NONE, PC_ADVANCE, .handle = dec_d },
    { "LD D, n",        8,  OPERAND_BYTE, PC_ADVANCE, .handle = ld_d_n },
    { "RLA",            4,  OPERAND_NONE, PC_ADVANCE, .handle = rla },
    { "JR e",          12,  OPERAND_BYTE, PC_MANUAL,  .handle = jr_e },
    { "ADD HL, DE",     8,  OPERAND_NONE, PC_ADVANCE, .handle = add_hl_de },
    { "LD A, [DE]",     8,  OPERAND_NONE, PC_ADVANCE, .handle = ld_a_at_de },
    { "DEC DE",         8,  OPERAND_NONE, PC_ADVANCE, .handle = dec_de },
    { "INC E",          4,  OPERAND_NONE, PC_ADVANCE, .handle = inc_e },
    { "DEC E",          4,  OPERAND_NONE, PC_ADVANCE, .handle = dec_e },
    { "LD E, n",        8,  OPERAND_BYTE, PC_ADVANCE, .handle = ld_e_n },
    { "RRA",            4,  OPERAND_NONE, PC_ADVANCE, .handle = rra },
    { "JR NZ, e",       8,  OPERAND_BYTE, PC_MANUAL,  .handle = jr_nz_e },
    { "LD HL, nn",     12,  OPERAND_WORD, PC_ADVANCE, .handle = ld_hl_nn },
    { "LD [HL+], A",    8,  OPERAND_NONE, PC_ADVANCE, .handle = ldi_at_hl_a },
    { "INC HL",         8,  OPERAND_NONE, PC_ADVANCE, .handle = inc_hl },
    { "INC H",          4,  OPERAND_NONE, PC_ADVANCE, .handle = inc_h },
    { "DEC H",          4,  OPERAND_NONE, PC_ADVANCE, .handle = dec_h },
    { "LD H, n",        8,  OPERAND_BYTE, PC_ADVANCE, .handle = ld_h_n },
    { "DAA",            4,  OPERAND_NONE, PC_ADVANCE, .handle = daa },
    { "JR Z, e",        8,  OPERAND_BYTE, PC_MANUAL,  .handle = jr_z_e },
    { "ADD HL, HL",     8,  OPERAND_NONE, PC_ADVANCE, .handle = add_hl_hl },
    { "LD A, [HL+]",    8,  OPERAND_NONE, PC_ADVANCE, .handle = ldi_a_at_hl },
    { "DEC HL",         8,  OPERAND_NONE, PC_ADVANCE, .handle = dec_hl },
    { "INC L",          4,  OPERAND_NONE, PC_ADVANCE, .handle = inc_l },
    { "DEC L",          4,  OPERAND_NONE, PC_ADVANCE, .handle = dec_l },
    { "LD L, n",        8,  OPERAND_BYTE, PC_ADVANCE, .handle = ld_l_n },
    { "CPL",            4,  OPERAND_NONE, PC_ADVANCE, .handle = cpl },
    { "JR NC, e",       8,  OPERAND_BYTE, PC_MANUAL,  .handle = jr_nc_e },
    { "LD SP, nn",     12,  OPERAND_WORD, PC_ADVANCE, .handle = ld_sp_nn },
    { "LD [HL-], A",    8,  OPERAND_NONE, PC_ADVANCE, .handle = ldd_at_hl_a },
    { "INC SP",         8,  OPERAND_NONE, PC_ADVANCE, .handle = inc_sp },
    { "INC [HL]",      12,  OPERAND_NONE, PC_ADVANCE, .handle = inc_at_hl },
    { "DEC [HL]",      12,  OPERAND_NONE, PC_ADVANCE, .handle = dec_at_hl },
    { "LD [HL], n",    12,  OPERAND_BYTE, PC_ADVANCE, .handle = ld_at_hl_n },
    { "SCF",            4,  OPERAND_NONE, PC_ADVANCE, .handle = scf },
    { "JR C, e",        8,  OPERAND_BYTE, PC_MANUAL,  .handle = jr_c_e },
    { "ADD HL, SP",     8,  OPERAND_NONE, PC_ADVANCE, .handle = add_hl_sp },
    { "LD A, [HL-]",    8,  OPERAND_NONE, PC_ADVANCE, .handle = ldd_a_at_hl },
    { "DEC SP",         8,  OPERAND_NONE, PC_ADVANCE, .handle = dec_sp },
    { "INC A",          4,  OPERAND_NONE, PC_ADVANCE, .handle = inc_a },
    { "DEC A",          4,  OPERAND_NONE, PC_ADVANCE, .handle = dec_a },
    { "LD A, n",        8,  OPERAND_BYTE, PC_ADVANCE, .handle = ld_a_n },
    { "CCF",            4,  OPERAND_NONE, PC_ADVANCE, .handle = ccf },
    { "LD B, B",        4,  OPERAND_NONE, PC_ADVANCE, .handle = nop },
    { "LD B, C",        4,  OPERAND_NONE, PC_ADVANCE, .handle = ld_b_c },
    { "LD B, D",        4,  OPERAND_NONE, PC_ADVANCE, .handle = ld_b_d },
    { "LD B, E",        4,  OPERAND_NONE, PC_ADVANCE, .handle = ld_b_e },
    { "LD B, H",        4,  OPERAND_NONE, PC_ADVANCE, .handle = ld_b_h },
    { "LD B, L",        4,  OPERAND_NONE, PC_ADVANCE, .handle = ld_b_l },
    { "LD B, [HL]",     8,  OPERAND_NONE, PC_ADVANCE, .handle = ld_b_at_hl },
    { "LD B, A",        4,  OPERAND_NONE, PC_ADVANCE, .handle = ld_b_a },
    { "LD C, B",        4,  OPERAND_NONE, PC_ADVANCE, .handle = ld_c_b },
    { "LD C, C",        4,  OPERAND_NONE, PC_ADVANCE, .handle = nop },
    { "LD C, D",        4,  OPERAND_NONE, PC_ADVANCE, .handle = ld_c_d },
    { "LD C, E",        4,  OPERAND_NONE, PC_ADVANCE, .handle = ld_c_e },
    { "LD C, H",        4,  OPERAND_NONE, PC_ADVANCE, .handle = ld_c_h },
    { "LD C, L",        4,  OPERAND_NONE, PC_ADVANCE, .handle = ld_c_l },
    { "LD C, [HL]",     8,  OPERAND_NONE, PC_ADVANCE, .handle = ld_c_at_hl },
    { "LD C, A",        4,  OPERAND_NONE, PC_ADVANCE, .handle = ld_c_a },
    { "LD D, B",        4,  OPERAND_NONE, PC_ADVANCE, .handle = ld_d_b },
    { "LD D, C",        4,  OPERAND_NONE, PC_ADVANCE, .handle = ld_d_c },
    { "LD D, D",        4,  OPERAND_NONE, PC_ADVANCE, .handle = nop },
    { "LD D, E",        4,  OPERAND_NONE, PC_ADVANCE, .handle = ld_d_e },
    { "LD D, H",        4,  OPERAND_NONE, PC_ADVANCE, .handle = ld_d_h },
    { "LD D, L",        4,  OPERAND_NONE, PC_ADVANCE, .handle = ld_d_l },
    { "LD D, [HL]",     8,  OPERAND_NONE, PC_ADVANCE, .handle = ld_d_at_hl },
    { "LD D, A",        4,  OPERAND_NONE, PC_ADVANCE, .handle = ld_d_a },
    { "LD E, B",        4,  OPERAND_NONE, PC_ADVANCE, .handle = ld_e_b },
    { "LD E, C",        4,  OPERAND_NONE, PC_ADVANCE, .handle = ld_e_c },
    { "LD E, D",        4,  OPERAND_NONE, PC_ADVANCE, .handle = ld_e_d },
    { "LD E, E",        4,  OPERAND_NONE, PC_ADVANCE, .handle = nop },
    { "LD E, H",        4,  OPERAND_NONE, PC_ADVANCE, .handle = ld_e_h },
    { "LD E, L",        4,  OPERAND_NONE, PC_ADVANCE, .handle = ld_e_l },
    { "LD E, [HL]",     8,  OPERAND_NONE, PC_ADVANCE, .handle = ld_e_at_hl },
    { "LD E, A",        4,  OPERAND_NONE, PC_ADVANCE, .handle = ld_e_a },
    { "LD H, B",        4,  OPERAND_NONE, PC_ADVANCE, .handle = ld_h_b },
    { "LD H, C",        4,  OPERAND_NONE, PC_ADVANCE, .handle = ld_h_c },
    { "LD H, D",        4,  OPERAND_NONE, PC_ADVANCE, .handle = ld_h_d },
    { "LD H, E",        4,  OPERAND_NONE, PC_ADVANCE, .handle = ld_h_e },
    { "LD H, H",        4,  OPERAND_NONE, PC_ADVANCE, .handle = nop },
    { "LD H, L",        4,  OPERAND_NONE, PC_ADVANCE, .handle = ld_h_l },
    { "LD H, [HL]",     8,  OPERAND_NONE, PC_ADVANCE, .handle = ld_h_at_hl },
    { "LD H, A",        4,  OPERAND_NONE, PC_ADVANCE, .handle = ld_h_a },
    { "LD L, B",        4,  OPERAND_NONE, PC_ADVANCE, .handle = ld_l_b },
    { "LD L, C",        4,  OPERAND_NONE, PC_ADVANCE, .handle = ld_l_c },
    { "LD L, D",        4,  OPERAND_NONE, PC_ADVANCE, .handle = ld_l_d },
    { "LD L, E",        4,  OPERAND_NONE, PC_ADVANCE, .handle = ld_l_e },
    { "LD L, H",        4,  OPERAND_NONE, PC_ADVANCE, .handle = ld_l_h },
    { "LD L, L",        4,  OPERAND_NONE, PC_ADVANCE, .handle = nop },
    { "LD L, [HL]",     8,  OPERAND_NONE, PC_ADVANCE, .handle = ld_l_at_hl },
    { "LD L, A",        4,  OPERAND_NONE, PC_ADVANCE, .handle = ld_l_a },
    { "LD [HL], B",     8,  OPERAND_NONE, PC_ADVANCE, .handle = ld_at_hl_b },
    { "LD [HL], C",     8,  OPERAND_NONE, PC_ADVANCE, .handle = ld_at_hl_c },
    { "LD [HL], D",     8,  OPERAND_NONE, PC_ADVANCE, .handle = ld_at_hl_d },
    { "LD [HL], E",     8,  OPERAND_NONE, PC_ADVANCE, .handle = ld_at_hl_e },
    { "LD [HL], H",     8,  OPERAND_NONE, PC_ADVANCE, .handle = ld_at_hl_h },
    { "LD [HL], L",     8,  OPERAND_NONE, PC_ADVANCE, .handle = ld_at_hl_l },
    { "HALT",           4,  OPERAND_NONE, PC_ADVANCE, .handle = halt },
    { "LD [HL], A",     8,  OPERAND_NONE, PC_ADVANCE, .handle = ld_at_hl_a },
    { "LD A, B",        4,  OPERAND_NONE, PC_ADVANCE, .handle = ld_a_b },
    { "LD A, C",        4,  OPERAND_NONE, PC_ADVANCE, .handle = ld_a_c },
    { "LD A, D",        4,  OPERAND_NONE, PC_ADVANCE, .handle = ld_a_d },
    { "LD A, E",        4,  OPERAND_NONE, PC_ADVANCE, .handle = ld_a_e },
    { "LD A, H",        4,  OPERAND_NONE, PC_ADVANCE, .handle = ld_a_h },
    { "LD A, L",        4,  OPERAND_NONE, PC_ADVANCE, .handle = ld_a_l },
    { "LD A, [HL]",     8,  OPERAND_NONE, PC_ADVANCE, .handle = ld_a_at_hl },
    { "LD A, A",        4,  OPERAND_NONE, PC_ADVANCE, .handle = nop },
    { "ADD A, B",       4,  OPERAND_NONE, PC_ADVANCE, .handle = add_a_b },
    { "ADD A, C",       4,  OPERAND_NONE, PC_ADVANCE, .handle = add_a_c },
    { "ADD A, D",       4,  OPERAND_NONE, PC_ADVANCE, .handle = add_a_d },
    { "ADD A, E",       4,  OPERAND_NONE, PC_ADVANCE, .handle = add_a_e },
    { "ADD A, H",       4,  OPERAND_NONE, PC_ADVANCE, .handle = add_a_h },
    { "ADD A, L",       4,  OPERAND_NONE, PC_ADVANCE, .handle = add_a_l },
    { "ADD A, [HL]",    8,  OPERAND_NONE, PC_ADVANCE, .handle = add_a_at_hl },
    { "ADD A, A",       4,  OPERAND_NONE, PC_ADVANCE, .handle = add_a_a },
    { "ADC A, B",       4,  OPERAND_NONE, PC_ADVANCE, .handle = adc_a_b },
    { "ADC A, C",       4,  OPERAND_NONE, PC_ADVANCE, .handle = adc_a_c },
    { "ADC A, D",       4,  OPERAND_NONE, PC_ADVANCE, .handle = adc_a_d },
    { "ADC A, E",       4,  OPERAND_NONE, PC_ADVANCE, .handle = adc_a_e },
    { "ADC A, H",       4,  OPERAND_NONE, PC_ADVANCE, .handle = adc_a_h },
    { "ADC A, L",       4,  OPERAND_NONE, PC_ADVANCE, .handle = adc_a_l },
    { "ADC A, [HL]",    8,  OPERAND_NONE, PC_ADVANCE, .handle = adc_a_at_hl },
    { "ADC A, A",       4,  OPERAND_NONE, PC_ADVANCE, .handle = adc_a_a },
    { "SUB A, B",       4,  OPERAND_NONE, PC_ADVANCE, .handle = sub_a_b },
    { "SUB A, C",       4,  OPERAND_NONE, PC_ADVANCE, .handle = sub_a_c },
    { "SUB A, D",       4,  OPERAND_NONE, PC_ADVANCE, .handle = sub_a_d },
    { "SUB A, E",       4,  OPERAND_NONE, PC_ADVANCE, .handle = sub_a_e },
    { "SUB A, H",       4,  OPERAND_NONE, PC_ADVANCE, .handle = sub_a_h },
    { "SUB A, L",       4,  OPERAND_NONE, PC_ADVANCE, .handle = sub_a_l },
    { "SUB A, [HL]",    8,  OPERAND_NONE, PC_ADVANCE, .handle = sub_a_at_hl },
    { "SUB A, A",       4,  OPERAND_NONE, PC_ADVANCE, .handle = sub_a_a },
    { "SBC A, B",       4,  OPERAND_NONE, PC_ADVANCE, .handle = sbc_a_b },
    { "SBC A, C",       4,  OPERAND_NONE, PC_ADVANCE, .handle = sbc_a_c },
    { "SBC A, D",       4,  OPERAND_NONE, PC_ADVANCE, .handle = sbc_a_d },
    { "SBC A, E",       4,  OPERAND_NONE, PC_ADVANCE, .handle = sbc_a_e },
    { "SBC A, H",       4,  OPERAND_NONE, PC_ADVANCE, .handle = sbc_a_h },
    { "SBC A, L",       4,  OPERAND_NONE, PC_ADVANCE, .handle = sbc_a_l },
    { "SBC A, [HL]",    8,  OPERAND_NONE, PC_ADVANCE, .handle = sbc_a_at_hl },
    { "SBC A, A",       4,  OPERAND_NONE, PC_ADVANCE, .handle = sbc_a_a },
    { "AND A, B",       4,  OPERAND_NONE, PC_ADVANCE, .handle = and_a_b },
    { "AND A, C",       4,  OPERAND_NONE, PC_ADVANCE, .handle = and_a_c },
    { "AND A, D",       4,  OPERAND_NONE, PC_ADVANCE, .handle = and_a_d },
    { "AND A, E",       4,  OPERAND_NONE, PC_ADVANCE, .handle = and_a_e },
    { "AND A, H",       4,  OPERAND_NONE, PC_ADVANCE, .handle = and_a_h },
    { "AND A, L",       4,  OPERAND_NONE, PC_ADVANCE, .handle = and_a_l },
    { "AND A, [HL]",    8,  OPERAND_NONE, PC_ADVANCE, .handle = and_a_at_hl },
    { "AND A, A",       4,  OPERAND_NONE, PC_ADVANCE, .handle = and_a_a },
    { "XOR A, B",       4,  OPERAND_NONE, PC_ADVANCE, .handle = xor_a_b },
    { "XOR A, C",       4,  OPERAND_NONE, PC_ADVANCE, .handle = xor_a_c },
    { "XOR A, D",       4,  OPERAND_NONE, PC_ADVANCE, .handle = xor_a_d },
    { "XOR A, E",       4,  OPERAND_NONE, PC_ADVANCE, .handle = xor_a_e },
    { "XOR A, H",       4,  OPERAND_NONE, PC_ADVANCE, .handle = xor_a_h },
    { "XOR A, L",       4,  OPERAND_NONE, PC_ADVANCE, .handle = xor_a_l },
    { "XOR A, [HL]",    8,  OPERAND_NONE, PC_ADVANCE, .handle = xor_a_at_hl },
    { "XOR A, A",       4,  OPERAND_NONE, PC_ADVANCE, .handle = xor_a_a },
    { "OR A, B",        4,  OPERAND_NONE, PC_ADVANCE, .handle = or_a_b },
    { "OR A, C",        4,  OPERAND_NONE, PC_ADVANCE, .handle = or_a_c },
    { "OR A, D",        4,  OPERAND_NONE, PC_ADVANCE, .handle = or_a_d },
    { "OR A, E",        4,  OPERAND_NONE, PC_ADVANCE, .handle = or_a_e },
    { "OR A, H",        4,  OPERAND_NONE, PC_ADVANCE, .handle = or_a_h },
    { "OR A, L",        4,  OPERAND_NONE, PC_ADVANCE, .handle = or_a_l },
    { "OR A, [HL]",     8,  OPERAND_NONE, PC_ADVANCE, .handle = or_a_at_hl },
    { "OR A, A",        4,  OPERAND_NONE, PC_ADVANCE, .handle = or_a_a },
    { "CP A, B",        4,  OPERAND_NONE, PC_ADVANCE, .handle = cp_a_b },
    { "CP A, C",        4,  OPERAND_NONE, PC_ADVANCE, .handle = cp_a_c },
    { "CP A, D",        4,  OPERAND_NONE, PC_ADVANCE, .handle = cp_a_d },
    { "CP A, E",        4,  OPERAND_NONE, PC_ADVANCE, .handle = cp_a_e },
    { "CP A, H",        4,  OPERAND_NONE, PC_ADVANCE, .handle = cp_a_h },
    { "CP A, L",        4,  OPERAND_NONE, PC_ADVANCE, .handle = cp_a_l },
    { "CP A, [HL]",     8,  OPERAND_NONE, PC_ADVANCE, .handle = cp_a_at_hl },
    { "CP A, A",        4,  OPERAND_NONE, PC_ADVANCE, .handle = cp_a_a },
    { "RET NZ",         8,  OPERAND_NONE, PC_MANUAL,  .handle = ret_nz },
    { "POP BC",        12,  OPERAND_NONE, PC_ADVANCE, .handle = pop_bc },
    { "JP NZ, nn",     12,  OPERAND_WORD, PC_MANUAL,  .handle = jp_nz_nn },
    { "JP nn",         16,  OPERAND_WORD, PC_MANUAL,  .handle = jp_nn },
    { "CALL NZ, nn",   12,  OPERAND_WORD, PC_MANUAL,  .handle = call_nz_nn },
    { "PUSH BC",       16,  OPERAND_NONE, PC_ADVANCE, .handle = push_bc },
    { "ADD A, n",       8,  OPERAND_BYTE, PC_ADVANCE, .handle = add_a_n },
    { "RST $00",       16,  OPERAND_NONE, PC_ADVANCE, .handle = rst_00 },
    { "RET Z",          8,  OPERAND_NONE, PC_MANUAL,  .handle = ret_z },
    { "RET",           16,  OPERAND_NONE, PC_MANUAL,  .handle = ret },
    { "JP Z, nn",      12,  OPERAND_WORD, PC_MANUAL,  .handle = jp_z_nn },
    { "CB n",           0,  OPERAND_BYTE, PC_ADVANCE, .handle = cb_n },
    { "CALL Z, nn",    12,  OPERAND_WORD, PC_MANUAL,  .handle = call_z_nn },
    { "CALL nn",       24,  OPERAND_WORD, PC_MANUAL,  .handle = call_nn },
    { "ADC A, n",       8,  OPERAND_BYTE, PC_ADVANCE, .handle = adc_a_n },
    { "RST $08",       16,  OPERAND_NONE, PC_ADVANCE, .handle = rst_08 },
    { "RET NC",         8,  OPERAND_NONE, PC_MANUAL,  .handle = ret_nc },
    { "POP DE",        12,  OPERAND_NONE, PC_ADVANCE, .handle = pop_de },
    { "JP NC, nn",     12,  OPERAND_WORD, PC_MANUAL,  .handle = jp_nc_nn },
    { "UNKNOWN",        0,  OPERAND_NONE, PC_MANUAL,  .handle = NULL },
    { "CALL NC, nn",   12,  OPERAND_WORD, PC_MANUAL,  .handle = call_nc_nn },
    { "PUSH DE",       16,  OPERAND_NONE, PC_ADVANCE, .handle = push_de },
    { "SUB A, n",       8,  OPERAND_BYTE, PC_ADVANCE, .handle = sub_a_n },
    { "RST $10",       16,  OPERAND_NONE, PC_ADVANCE, .handle = rst_10 },
    { "RET C",          8,  OPERAND_NONE, PC_MANUAL,  .handle = ret_c },
    { "RETI",          16,  OPERAND_NONE, PC_ADVANCE, .handle = reti },
    { "JP C, nn",      12,  OPERAND_WORD, PC_MANUAL,  .handle = jp_c_nn },
    { "UNKNOWN",        0,  OPERAND_NONE, PC_MANUAL,  .handle = NULL },
    { "CALL C, nn",    12,  OPERAND_WORD, PC_MANUAL,  .handle = call_c_nn },
    { "UNKNOWN",        0,  OPERAND_NONE, PC_MANUAL,  .handle = NULL },
    { "SBC A, N",       8,  OPERAND_BYTE, PC_ADVANCE, .handle = sbc_a_n },
    { "RST $18",       16,  OPERAND_NONE, PC_ADVANCE, .handle = rst_18 },
    { "LDH [n], A",    12,  OPERAND_BYTE, PC_ADVANCE, .handle = ldh_at_n_a },
    { "POP HL",        12,  OPERAND_NONE, PC_ADVANCE, .handle = pop_hl },
    { "LDH [C], A",     8,  OPERAND_NONE, PC_ADVANCE, .handle = ldh_at_c_a },
    { "UNKNOWN",        0,  OPERAND_NONE, PC_MANUAL,  .handle = NULL },
    { "UNKNOWN",        0,  OPERAND_NONE, PC_MANUAL,  .handle = NULL },
    { "PUSH HL",       16,  OPERAND_NONE, PC_ADVANCE, .handle = push_hl },
    { "AND A, n",       8,  OPERAND_BYTE, PC_ADVANCE, .handle = and_a_n },
    { "RST $20",       16,  OPERAND_NONE, PC_ADVANCE, .handle = rst_20 },
    { "ADD SP, n",     16,  OPERAND_BYTE, PC_ADVANCE, .handle = add_sp_n },
    { "JP HL",          4,  OPERAND_NONE, PC_MANUAL,  .handle = jp_hl },
    { "LD [nn], A",    16,  OPERAND_WORD, PC_ADVANCE, .handle = ld_at_nn_a },
    { "UNKNOWN",        0,  OPERAND_NONE, PC_MANUAL,  .handle = NULL },
    { "UNKNOWN",        0,  OPERAND_NONE, PC_MANUAL,  .handle = NULL },
    { "UNKNOWN",        0,  OPERAND_NONE, PC_MANUAL,  .handle = NULL },
    { "XOR A, n",       8,  OPERAND_BYTE, PC_ADVANCE, .handle = xor_a_n },
    { "RST $28",       16,  OPERAND_NONE, PC_ADVANCE, .handle = rst_28 },
    { "LDH A, [n]",    12,  OPERAND_BYTE, PC_ADVANCE, .handle = ldh_a_at_n },
    { "POP AF",        12,  OPERAND_NONE, PC_ADVANCE, .handle = pop_af },
    { "LDH A, [C]",     8,  OPERAND_NONE, PC_ADVANCE, .handle = ldh_a_at_c },
    { "DI",             4,  OPERAND_NONE, PC_ADVANCE, .handle = di },
    { "UNKNOWN",        0,  OPERAND_NONE, PC_MANUAL,  .handle = NULL },
    { "PUSH AF",       16,  OPERAND_NONE, PC_ADVANCE, .handle = push_af },
    { "OR A, n",        8,  OPERAND_BYTE, PC_ADVANCE, .handle = or_a_n },
    { "RST $30",       16,  OPERAND_NONE, PC_ADVANCE, .handle = rst_30 },
    { "LD HL, SP+n",   12,  OPERAND_BYTE, PC_ADVANCE, .handle = ld_hl_sp_plus_n },
    { "LD SP, HL",      8,  OPERAND_NONE, PC_ADVANCE, .handle = ld_sp_hl },
    { "LD A, [nn]",    16,  OPERAND_WORD, PC_ADVANCE, .handle = ld_a_at_nn },
    { "EI",             4,  OPERAND_NONE, PC_ADVANCE, .handle = ei },
    { "UNKNOWN",        0,  OPERAND_NONE, PC_MANUAL,  .handle = NULL },
    { "UNKNOWN",        0,  OPERAND_NONE, PC_MANUAL,  .handle = NULL },
    { "CP A, n",        8,  OPERAND_BYTE, PC_ADVANCE, .handle = cp_a_n },
    { "RST $38",       16,  OPERAND_NONE, PC_ADVANCE, .handle = rst_38 },
};

void execute(Cpu* cpu, Memory* mem, uint8_t byte)
{
    if (byte >= 0x100)
        return;

    Instruction instruction = instructions[byte];

    instruction.operand = 0;
    instruction.operand16 = 0;

    switch (instruction.operand_size)
    {
        case OPERAND_BYTE:
            instruction.operand = memory_read(mem, cpu->pc);
            break;
        case OPERAND_WORD:
            instruction.operand16 = memory_read16(mem, cpu->pc);
            break;
    };

    instruction.handle(&instruction, cpu, mem);

    if (instruction.pc_mode == PC_ADVANCE && instruction.operand_size != OPERAND_NONE)
        cpu_advance_pc(cpu, instruction.operand_size);

    cpu_add_ticks(cpu, instruction.base_ticks);
}