#include "../inc/interrupts.h"

void handle_interrupts(Cpu* cpu, Memory* mem)
{
    if (cpu->state == CPU_HALTED)
        cpu->state = CPU_RUNNING;

    if (!cpu->ime)
        return;

    uint8_t IE = memory_read(mem, IE_ADDR);
    uint8_t IF = memory_read(mem, IF_ADDR);
    uint8_t requested = IE & IF;

    if (requested)
    {
        cpu->ime = 0;

        if (requested & VBLANK_INTERRUPT)
        {
            memory_write(mem, IF_ADDR, IF & ~(VBLANK_INTERRUPT));
            cpu_call(cpu, mem, VBLANK_ADDR);
        }
        else if (requested & LCD_STAT_INTERRUPT)
        {
            memory_write(mem, IF_ADDR, IF & ~(LCD_STAT_INTERRUPT));
            cpu_call(cpu, mem, LCD_STAT_ADDR);
        }
        else if (requested & TIMER_INTERRUPT)
        {
            memory_write(mem, IF_ADDR, IF & ~(TIMER_INTERRUPT));
            cpu_call(cpu, mem, TIMER_ADDR);
        }
        else if (requested & SERIAL_INTERRUPT)
        {
            memory_write(mem, IF_ADDR, IF & ~(SERIAL_INTERRUPT));
            cpu_call(cpu, mem, SERIAL_ADDR);
        }
        else if (requested & JOYPAD_INTERRUPT)
        {
            memory_write(mem, IF_ADDR, IF & ~(JOYPAD_INTERRUPT));
            cpu_call(cpu, mem, JOYPAD_ADDR);
        }
    }
}

void request_interrupt(Memory* mem, uint8_t interrupt)
{
    uint8_t IF = memory_read(mem, IF_ADDR);
    IF |= interrupt;
    memory_write(mem, IF_ADDR, IF & 0x1F);
}