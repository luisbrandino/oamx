#include <stdlib.h>
#include <assert.h>
#include "../inc/instructions.h"

void test_instructions_increment()
{
    Cpu* cpu = cpu_init();
    Memory* mem = memory_init();
    cpu_reset(cpu);

    CLEAR_FLAG(FLAG_CARRY | FLAG_HALFCARRY | FLAG_ZERO | FLAG_NEGATIVE);

    cpu->b = 0x05;
    execute(cpu, mem, 0x04);
    assert(cpu->b == 0x06);
    assert(IS_FLAG_SET(FLAG_CARRY) == 0);
    assert(IS_FLAG_SET(FLAG_HALFCARRY) == 0);
    assert(IS_FLAG_SET(FLAG_ZERO) == 0);
    assert(IS_FLAG_SET(FLAG_NEGATIVE) == 0);

    CLEAR_FLAG(FLAG_CARRY | FLAG_HALFCARRY | FLAG_ZERO | FLAG_NEGATIVE);

    cpu->b = 0x0F;
    execute(cpu, mem, 0x04);
    assert(cpu->b == 0x10);
    assert(IS_FLAG_SET(FLAG_CARRY) == 0);
    assert(IS_FLAG_SET(FLAG_HALFCARRY) == 1);
    assert(IS_FLAG_SET(FLAG_ZERO) == 0);
    assert(IS_FLAG_SET(FLAG_NEGATIVE) == 0);
}

void test_instructions_decrement()
{
    Cpu* cpu = cpu_init();
    Memory* mem = memory_init();
    cpu_reset(cpu);

    CLEAR_FLAG(FLAG_CARRY | FLAG_HALFCARRY | FLAG_ZERO | FLAG_NEGATIVE);

    cpu->b = 0x05;
    execute(cpu, mem, 0x05);
    assert(cpu->b == 0x04);
    assert(IS_FLAG_SET(FLAG_CARRY) == 0);
    assert(IS_FLAG_SET(FLAG_HALFCARRY) == 0);
    assert(IS_FLAG_SET(FLAG_ZERO) == 0);
    assert(IS_FLAG_SET(FLAG_NEGATIVE) == 1);

    CLEAR_FLAG(FLAG_CARRY | FLAG_HALFCARRY | FLAG_ZERO | FLAG_NEGATIVE);

    cpu->b = 0x10;
    execute(cpu, mem, 0x05);
    assert(cpu->b == 0x0F);
    assert(IS_FLAG_SET(FLAG_CARRY) == 0);
    assert(IS_FLAG_SET(FLAG_HALFCARRY) == 1);
    assert(IS_FLAG_SET(FLAG_ZERO) == 0);
    assert(IS_FLAG_SET(FLAG_NEGATIVE) == 1);
}

void test_instructions_add()
{
    Cpu* cpu = cpu_init();
    Memory* mem = memory_init();
    cpu_reset(cpu);

    CLEAR_FLAG(FLAG_CARRY | FLAG_HALFCARRY | FLAG_ZERO | FLAG_NEGATIVE);

    cpu->a = 0x05;
    cpu->b = 0x05;
    execute(cpu, mem, 0x80);
    assert(cpu->a == 0x0A);
    assert(IS_FLAG_SET(FLAG_CARRY) == 0);
    assert(IS_FLAG_SET(FLAG_HALFCARRY) == 0);
    assert(IS_FLAG_SET(FLAG_ZERO) == 0);
    assert(IS_FLAG_SET(FLAG_NEGATIVE) == 0);

    CLEAR_FLAG(FLAG_CARRY | FLAG_HALFCARRY | FLAG_ZERO | FLAG_NEGATIVE);

    cpu->a = 0x0F;
    cpu->b = 0x01;
    execute(cpu, mem, 0x80);
    assert(cpu->a == 0x10);
    assert(IS_FLAG_SET(FLAG_CARRY) == 0);
    assert(IS_FLAG_SET(FLAG_HALFCARRY) == 1);
    assert(IS_FLAG_SET(FLAG_ZERO) == 0);
    assert(IS_FLAG_SET(FLAG_NEGATIVE) == 0);

    CLEAR_FLAG(FLAG_CARRY | FLAG_HALFCARRY | FLAG_ZERO | FLAG_NEGATIVE);

    cpu->a = 0xF0;
    cpu->b = 0x10;
    execute(cpu, mem, 0x80);
    assert(cpu->a == 0x00);
    assert(IS_FLAG_SET(FLAG_CARRY) == 1);
    assert(IS_FLAG_SET(FLAG_HALFCARRY) == 0);
    assert(IS_FLAG_SET(FLAG_ZERO) == 1);
    assert(IS_FLAG_SET(FLAG_NEGATIVE) == 0);

    CLEAR_FLAG(FLAG_CARRY | FLAG_HALFCARRY | FLAG_ZERO | FLAG_NEGATIVE);

    cpu->a = 0xFF;
    cpu->b = 0xFF;
    execute(cpu, mem, 0x80);
    assert(cpu->a == 0xFE);
    assert(IS_FLAG_SET(FLAG_CARRY) == 1);
    assert(IS_FLAG_SET(FLAG_HALFCARRY) == 1);
    assert(IS_FLAG_SET(FLAG_ZERO) == 0);
    assert(IS_FLAG_SET(FLAG_NEGATIVE) == 0);

    CLEAR_FLAG(FLAG_CARRY | FLAG_HALFCARRY | FLAG_ZERO | FLAG_NEGATIVE);
    cpu->a = 0x3C; 
    cpu->b = 0x42; 
    execute(cpu, mem, 0x80);
    assert(cpu->a == 0x7E);
    assert(IS_FLAG_SET(FLAG_CARRY) == 0);
    assert(IS_FLAG_SET(FLAG_HALFCARRY) == 0);
    assert(IS_FLAG_SET(FLAG_ZERO) == 0);
    assert(IS_FLAG_SET(FLAG_NEGATIVE) == 0);

    CLEAR_FLAG(FLAG_CARRY | FLAG_HALFCARRY | FLAG_ZERO | FLAG_NEGATIVE);
    cpu->a = 0x29;
    cpu->b = 0x17;
    execute(cpu, mem, 0x80);
    assert(cpu->a == 0x40);
    assert(IS_FLAG_SET(FLAG_CARRY) == 0);
    assert(IS_FLAG_SET(FLAG_HALFCARRY) == 1); 
    assert(IS_FLAG_SET(FLAG_ZERO) == 0);
    assert(IS_FLAG_SET(FLAG_NEGATIVE) == 0);

    CLEAR_FLAG(FLAG_CARRY | FLAG_HALFCARRY | FLAG_ZERO | FLAG_NEGATIVE);
    cpu->a = 0x00;
    cpu->b = 0x00;
    execute(cpu, mem, 0x80); 
    assert(cpu->a == 0x00);
    assert(IS_FLAG_SET(FLAG_CARRY) == 0);
    assert(IS_FLAG_SET(FLAG_HALFCARRY) == 0);
    assert(IS_FLAG_SET(FLAG_ZERO) == 1);
    assert(IS_FLAG_SET(FLAG_NEGATIVE) == 0);
}

void test_instructions_add16()
{
    Cpu* cpu = cpu_init();
    Memory* mem = memory_init();
    cpu_reset(cpu);

    CLEAR_FLAG(FLAG_CARRY | FLAG_HALFCARRY | FLAG_ZERO | FLAG_NEGATIVE);

    set_hl(cpu, 0x1000);
    set_bc(cpu, 0x1000);
    execute(cpu, mem, 0x09);
    assert(get_hl(cpu) == 0x2000);
    assert(IS_FLAG_SET(FLAG_CARRY) == 0);
    assert(IS_FLAG_SET(FLAG_HALFCARRY) == 0);
    assert(IS_FLAG_SET(FLAG_NEGATIVE) == 0);

    CLEAR_FLAG(FLAG_CARRY | FLAG_HALFCARRY | FLAG_ZERO | FLAG_NEGATIVE);

    set_hl(cpu, 0x0FFF);
    set_bc(cpu, 0x0001);
    execute(cpu, mem, 0x09);
    assert(get_hl(cpu) == 0x1000);
    assert(IS_FLAG_SET(FLAG_CARRY) == 0);
    assert(IS_FLAG_SET(FLAG_HALFCARRY) == 1);
    assert(IS_FLAG_SET(FLAG_NEGATIVE) == 0);

    CLEAR_FLAG(FLAG_CARRY | FLAG_HALFCARRY | FLAG_ZERO | FLAG_NEGATIVE);

    set_hl(cpu, 0x0FFE);
    set_bc(cpu, 0x0001);
    execute(cpu, mem, 0x09);
    assert(get_hl(cpu) == 0x0FFF);
    assert(IS_FLAG_SET(FLAG_CARRY) == 0);
    assert(IS_FLAG_SET(FLAG_HALFCARRY) == 0);
    assert(IS_FLAG_SET(FLAG_NEGATIVE) == 0);

    CLEAR_FLAG(FLAG_CARRY | FLAG_HALFCARRY | FLAG_ZERO | FLAG_NEGATIVE);

    set_hl(cpu, 0xFFFF);
    set_bc(cpu, 0x0001);
    execute(cpu, mem, 0x09);
    assert(get_hl(cpu) == 0x0000);
    assert(IS_FLAG_SET(FLAG_CARRY) == 1);
    assert(IS_FLAG_SET(FLAG_HALFCARRY) == 1);
    assert(IS_FLAG_SET(FLAG_NEGATIVE) == 0);

    CLEAR_FLAG(FLAG_CARRY | FLAG_HALFCARRY | FLAG_ZERO | FLAG_NEGATIVE);

    set_hl(cpu, 0xF000);
    set_bc(cpu, 0xF000);
    execute(cpu, mem, 0x09);

    assert(get_hl(cpu) == 0xE000);
    assert(IS_FLAG_SET(FLAG_CARRY) == 1);
    assert(IS_FLAG_SET(FLAG_HALFCARRY) == 0);
    assert(IS_FLAG_SET(FLAG_NEGATIVE) == 0);

    SET_FLAG(FLAG_ZERO);
    CLEAR_FLAG(FLAG_CARRY | FLAG_HALFCARRY | FLAG_NEGATIVE);

    set_hl(cpu, 0x0001);
    set_bc(cpu, 0x0001);
    execute(cpu, mem, 0x09);

    assert(get_hl(cpu) == 0x0002);
    assert(IS_FLAG_SET(FLAG_ZERO) == 1);
    assert(IS_FLAG_SET(FLAG_CARRY) == 0);
    assert(IS_FLAG_SET(FLAG_HALFCARRY) == 0);
    assert(IS_FLAG_SET(FLAG_NEGATIVE) == 0);
}

void test_instructions_adc()
{
    Cpu* cpu = cpu_init();
    Memory* mem = memory_init();
    cpu_reset(cpu);

    CLEAR_FLAG(FLAG_CARRY | FLAG_HALFCARRY | FLAG_ZERO | FLAG_NEGATIVE);

    cpu->a = 0x05;
    cpu->b = 0x03;
    execute(cpu, mem, 0x88);

    assert(cpu->a == 0x08);
    assert(IS_FLAG_SET(FLAG_CARRY) == 0);
    assert(IS_FLAG_SET(FLAG_HALFCARRY) == 0);
    assert(IS_FLAG_SET(FLAG_ZERO) == 0);
    assert(IS_FLAG_SET(FLAG_NEGATIVE) == 0);

    CLEAR_FLAG(FLAG_CARRY | FLAG_HALFCARRY | FLAG_ZERO | FLAG_NEGATIVE);

    cpu->a = 0x0F;
    cpu->b = 0x01;
    execute(cpu, mem, 0x88);
    assert(cpu->a == 0x10);
    assert(IS_FLAG_SET(FLAG_HALFCARRY) == 1);
    assert(IS_FLAG_SET(FLAG_CARRY) == 0);
    assert(IS_FLAG_SET(FLAG_ZERO) == 0);
    assert(IS_FLAG_SET(FLAG_NEGATIVE) == 0);

    CLEAR_FLAG(FLAG_CARRY | FLAG_HALFCARRY | FLAG_ZERO | FLAG_NEGATIVE);

    cpu->a = 0xF0;
    cpu->b = 0x20;
    execute(cpu, mem, 0x88);
    assert(cpu->a == 0x10);
    assert(IS_FLAG_SET(FLAG_CARRY) == 1);
    assert(IS_FLAG_SET(FLAG_HALFCARRY) == 0);
    assert(IS_FLAG_SET(FLAG_ZERO) == 0);
    assert(IS_FLAG_SET(FLAG_NEGATIVE) == 0);

    CLEAR_FLAG(FLAG_CARRY | FLAG_HALFCARRY | FLAG_ZERO | FLAG_NEGATIVE);
    SET_FLAG(FLAG_CARRY);

    cpu->a = 0x05;
    cpu->b = 0x03;
    execute(cpu, mem, 0x88);
    assert(cpu->a == 0x09);
    assert(IS_FLAG_SET(FLAG_CARRY) == 0);
    assert(IS_FLAG_SET(FLAG_HALFCARRY) == 0);
    assert(IS_FLAG_SET(FLAG_ZERO) == 0);
    assert(IS_FLAG_SET(FLAG_NEGATIVE) == 0);

    CLEAR_FLAG(FLAG_CARRY | FLAG_HALFCARRY | FLAG_ZERO | FLAG_NEGATIVE);
    SET_FLAG(FLAG_CARRY);

    cpu->a = 0x0F;
    cpu->b = 0x00;
    execute(cpu, mem, 0x88);
    assert(cpu->a == 0x10);
    assert(IS_FLAG_SET(FLAG_HALFCARRY) == 1);
    assert(IS_FLAG_SET(FLAG_CARRY) == 0);
    assert(IS_FLAG_SET(FLAG_ZERO) == 0);
    assert(IS_FLAG_SET(FLAG_NEGATIVE) == 0);

    CLEAR_FLAG(FLAG_CARRY | FLAG_HALFCARRY | FLAG_ZERO | FLAG_NEGATIVE);
    SET_FLAG(FLAG_CARRY);

    cpu->a = 0xFF;
    cpu->b = 0x00;
    execute(cpu, mem, 0x88);
    assert(cpu->a == 0x00);
    assert(IS_FLAG_SET(FLAG_CARRY) == 1);
    assert(IS_FLAG_SET(FLAG_HALFCARRY) == 1);
    assert(IS_FLAG_SET(FLAG_ZERO) == 1);
    assert(IS_FLAG_SET(FLAG_NEGATIVE) == 0);

    CLEAR_FLAG(FLAG_CARRY | FLAG_HALFCARRY | FLAG_ZERO | FLAG_NEGATIVE);

    cpu->a = 0x8F;
    cpu->b = 0x91;
    execute(cpu, mem, 0x88);
    assert(cpu->a == 0x20);
    assert(IS_FLAG_SET(FLAG_CARRY) == 1);
    assert(IS_FLAG_SET(FLAG_HALFCARRY) == 1);
    assert(IS_FLAG_SET(FLAG_ZERO) == 0);
    assert(IS_FLAG_SET(FLAG_NEGATIVE) == 0);

    CLEAR_FLAG(FLAG_CARRY | FLAG_HALFCARRY | FLAG_ZERO | FLAG_NEGATIVE);

    cpu->a = 0x7F;
    cpu->b = 0x80;
    execute(cpu, mem, 0x88);
    assert(cpu->a == 0xFF);
    assert(IS_FLAG_SET(FLAG_CARRY) == 0);
    assert(IS_FLAG_SET(FLAG_HALFCARRY) == 0);
    assert(IS_FLAG_SET(FLAG_ZERO) == 0);
    assert(IS_FLAG_SET(FLAG_NEGATIVE) == 0);

    CLEAR_FLAG(FLAG_CARRY | FLAG_HALFCARRY | FLAG_ZERO | FLAG_NEGATIVE);
    SET_FLAG(FLAG_CARRY);

    cpu->a = 0xFE;
    cpu->b = 0x01;
    execute(cpu, mem, 0x88);
    assert(cpu->a == 0x00);
    assert(IS_FLAG_SET(FLAG_CARRY) == 1);
    assert(IS_FLAG_SET(FLAG_HALFCARRY) == 1);
    assert(IS_FLAG_SET(FLAG_ZERO) == 1);
    assert(IS_FLAG_SET(FLAG_NEGATIVE) == 0);

    CLEAR_FLAG(FLAG_CARRY | FLAG_HALFCARRY | FLAG_ZERO | FLAG_NEGATIVE);
    SET_FLAG(FLAG_CARRY);

    cpu->a = 0x00;
    cpu->b = 0x00;
    execute(cpu, mem, 0x88);
    assert(cpu->a == 0x01);
    assert(IS_FLAG_SET(FLAG_CARRY) == 0);
    assert(IS_FLAG_SET(FLAG_HALFCARRY) == 0);
    assert(IS_FLAG_SET(FLAG_ZERO) == 0);
    assert(IS_FLAG_SET(FLAG_NEGATIVE) == 0);

    CLEAR_FLAG(FLAG_CARRY | FLAG_HALFCARRY | FLAG_ZERO | FLAG_NEGATIVE);

    cpu->a = 0x00;
    cpu->b = 0x00;
    execute(cpu, mem, 0x88);
    assert(cpu->a == 0x00);
    assert(IS_FLAG_SET(FLAG_CARRY) == 0);
    assert(IS_FLAG_SET(FLAG_HALFCARRY) == 0);
    assert(IS_FLAG_SET(FLAG_ZERO) == 1);
    assert(IS_FLAG_SET(FLAG_NEGATIVE) == 0);
}

int main()
{
    test_instructions_increment();
    test_instructions_decrement();
    test_instructions_add();
    test_instructions_add16();
    test_instructions_adc();

    return EXIT_SUCCESS;
}