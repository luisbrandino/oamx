#include <stdlib.h>
#include <assert.h>
#include "../inc/interrupts.h"

void test_interrupts_request_interrupt()
{
    Memory* mem = memory_init();

    request_interrupt(mem, VBLANK_INTERRUPT);
    assert(memory_read(mem, IF_ADDR) == VBLANK_INTERRUPT);

    request_interrupt(mem, LCD_STAT_INTERRUPT);
    assert(memory_read(mem, IF_ADDR) == (VBLANK_INTERRUPT | LCD_STAT_INTERRUPT));

    request_interrupt(mem, TIMER_INTERRUPT);
    assert(memory_read(mem, IF_ADDR) == (VBLANK_INTERRUPT | LCD_STAT_INTERRUPT | TIMER_INTERRUPT));

    request_interrupt(mem, SERIAL_INTERRUPT);
    assert(memory_read(mem, IF_ADDR) == (VBLANK_INTERRUPT | LCD_STAT_INTERRUPT | TIMER_INTERRUPT | SERIAL_INTERRUPT));

    request_interrupt(mem, JOYPAD_INTERRUPT);
    assert(memory_read(mem, IF_ADDR) == (VBLANK_INTERRUPT | LCD_STAT_INTERRUPT | TIMER_INTERRUPT | SERIAL_INTERRUPT | JOYPAD_INTERRUPT));

    free(mem);
}

void test_interrupts_handle_interrupts()
{
    Cpu* cpu = cpu_init();
    Memory* mem = memory_init();
    cpu_reset(cpu);

    memory_write(mem, IF_ADDR, 0x00);
    memory_write(mem, IE_ADDR, 0x00);

    cpu->state = CPU_HALTED;
    cpu->ime = 0;
    memory_write(mem, IF_ADDR, VBLANK_INTERRUPT);
    memory_write(mem, IE_ADDR, 0x00);
    handle_interrupts(cpu, mem);
    assert(cpu->state == CPU_RUNNING);

    cpu->state = CPU_RUNNING;
    cpu->pc = 0x1234;

    const struct {
        uint8_t bit;
        uint16_t addr;
    } interrupts[5] = {
        { VBLANK_INTERRUPT, VBLANK_ADDR },
        { LCD_STAT_INTERRUPT, LCD_STAT_ADDR },
        { TIMER_INTERRUPT, TIMER_ADDR },
        { SERIAL_INTERRUPT, SERIAL_ADDR },
        { JOYPAD_INTERRUPT, JOYPAD_ADDR }
    };

    for (int i = 0; i < 5; i++) {
        cpu->pc = 0x1234;
        cpu->ime = 1;
        cpu->state = CPU_HALTED;
        memory_write(mem, IF_ADDR, interrupts[i].bit);
        memory_write(mem, IE_ADDR, interrupts[i].bit);

        handle_interrupts(cpu, mem);

        assert(cpu->ime == 0);
        assert(cpu->state == CPU_RUNNING);
        assert(cpu->pc == interrupts[i].addr);

        uint8_t IF = memory_read(mem, IF_ADDR);
        assert((IF & interrupts[i].bit) == 0);

        uint16_t sp_val = memory_read16(mem, cpu->sp);
        assert(sp_val == 0x1234);

        cpu->sp = 0xFFFE;
    }

    free(cpu);
    free(mem);
}

int main()
{
    test_interrupts_request_interrupt();
    test_interrupts_handle_interrupts();

    return EXIT_SUCCESS;
}