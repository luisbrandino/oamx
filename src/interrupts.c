#include "../inc/interrupts.h"
#include "../inc/display.h"

void handle_interrupts(Cpu* cpu, Ppu* ppu, Memory* mem)
{
    uint8_t requested = mem->IE & mem->IF;

    if (cpu->state == CPU_HALTED)
    {
        if (requested == 0)
            return;

        cpu->state = CPU_RUNNING;
    }

    if (!cpu->ime)
        return;

    if (requested)
    {
        cpu->ime = 0;

        if (requested & VBLANK_INTERRUPT)
        {
            mem->IF &= ~(VBLANK_INTERRUPT);
            cpu_call(cpu, mem, VBLANK_ADDR);
        }
        else if (requested & LCD_STAT_INTERRUPT)
        {
            mem->IF &= ~(LCD_STAT_INTERRUPT);
            cpu_call(cpu, mem, LCD_STAT_ADDR);
        }
        else if (requested & TIMER_INTERRUPT)
        {
            mem->IF &= ~(TIMER_INTERRUPT);
            cpu_call(cpu, mem, TIMER_ADDR);
        }
        else if (requested & SERIAL_INTERRUPT)
        {
            mem->IF &= ~(SERIAL_INTERRUPT);
            cpu_call(cpu, mem, SERIAL_ADDR);
        }
        else if (requested & JOYPAD_INTERRUPT)
        {
            mem->IF &= ~(JOYPAD_INTERRUPT);
            cpu_call(cpu, mem, JOYPAD_ADDR);
        }
    }
}

void request_interrupt(Memory* mem, uint8_t interrupt)
{
    mem->IF |= interrupt;
}