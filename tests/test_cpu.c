#include <stdlib.h>
#include <assert.h>
#include "../inc/cpu.h"

void test_cpu_reset()
{
    Cpu* cpu = cpu_init();
    cpu_reset(cpu);

    assert(get_af(cpu) == 0x01B0);
    assert(cpu->a == 0x01);
    assert(cpu->f == 0xB0);

    assert(get_bc(cpu) == 0x0013);
    assert(cpu->b  == 0x00);
    assert(cpu->c  == 0x13);

    assert(get_de(cpu) == 0x00D8);
    assert(cpu->d  == 0x00);
    assert(cpu->e  == 0xD8);

    assert(get_hl(cpu) == 0x014D);
    assert(cpu->h  == 0x01);
    assert(cpu->l  == 0x4d);

    assert(cpu->sp == 0xFFFE);
    assert(cpu->pc == 0x0100);

    free(cpu);
}

void test_cpu_push_and_pop()
{
    Cpu* cpu = cpu_init();
    Memory* mem = memory_init();
    cpu_reset(cpu);

    uint16_t initial_sp = cpu->sp;

    cpu_push(cpu, mem, 0x1CDE);

    assert(cpu->sp == initial_sp - 2);
    assert(memory_read16(mem, initial_sp - 2) == 0x1CDE);

    uint16_t value = cpu_pop(cpu, mem);

    assert(value == 0x1CDE);
    assert(cpu->sp == initial_sp);

    free(cpu);
    free(mem);
}

void test_cpu_call_and_ret()
{
    Cpu* cpu = cpu_init();
    Memory* mem = memory_init();
    cpu_reset(cpu);

    uint16_t initial_pc = cpu->pc;
    uint16_t initial_sp = cpu->sp;

    cpu_call(cpu, mem, 0x1000);
    assert(cpu->pc == 0x1000);
    assert(memory_read16(mem, initial_sp - 2) == initial_pc);

    cpu_ret(cpu, mem);
    assert(cpu->pc == initial_pc);
    assert(cpu->sp == initial_sp);

    free(cpu);
    free(mem);
}

void test_cpu_flags()
{
    Cpu* cpu = cpu_init();
    cpu_reset(cpu);

    SET_FLAG(FLAG_CARRY);
    assert(IS_FLAG_SET(FLAG_CARRY) == 1);

    SET_FLAG(FLAG_HALFCARRY);
    assert(IS_FLAG_SET(FLAG_HALFCARRY) == 1);

    SET_FLAG(FLAG_NEGATIVE);
    assert(IS_FLAG_SET(FLAG_NEGATIVE) == 1);

    SET_FLAG(FLAG_ZERO);
    assert(IS_FLAG_SET(FLAG_ZERO) == 1);

    CLEAR_FLAG(FLAG_CARRY | FLAG_HALFCARRY | FLAG_NEGATIVE | FLAG_ZERO);

    assert(IS_FLAG_SET(FLAG_CARRY) == 0);
    assert(IS_FLAG_SET(FLAG_HALFCARRY) == 0);
    assert(IS_FLAG_SET(FLAG_NEGATIVE) == 0);
    assert(IS_FLAG_SET(FLAG_ZERO) == 0);
}

int main()
{
    test_cpu_reset();
    test_cpu_push_and_pop();
    test_cpu_call_and_ret();
    test_cpu_flags();

    return EXIT_SUCCESS;
}