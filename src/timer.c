#include "../inc/timer.h"
#include "../inc/interrupts.h"

static const uint16_t TAC_FREQUENCIES[4] = { 1024, 16, 64, 256 };

static inline void increment_timer_register(Memory* mem, uint16_t addr) { memory_write(mem, addr, memory_read(mem, addr) + 1); }

void timer_update(Timer* timer, Memory* mem, uint16_t ticks)
{
    timer->div_counter += ticks;
    while (timer->div_counter >= DIV_TICKS)
    {
        timer->div_counter -= DIV_TICKS;
        increment_timer_register(mem, DIV_ADDR);
    }

    uint8_t TAC = memory_read(mem, TAC_ADDR);

    if (TAC & TAC_ENABLED)
    {
        uint16_t frequency = TAC_FREQUENCIES[TAC & 0x03];

        timer->tima_counter += ticks;
        while (timer->tima_counter >= frequency)
        {
            timer->tima_counter -= frequency;
            uint8_t TIMA = memory_read(mem, TIMA_ADDR);
            TIMA++;

            // overflow
            if (TIMA == 0x00) {
                TIMA = memory_read(mem, TMA_ADDR);
                request_interrupt(mem, TIMER_INTERRUPT);
            }

            memory_write(mem, TIMA_ADDR, TIMA);
        }
    }
}