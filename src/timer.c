#include "../inc/timer.h"
#include "../inc/interrupts.h"

static const uint16_t TAC_FREQUENCIES[4] = { 1024, 16, 64, 256 };

void timer_update(Timer* timer, Memory* mem, uint16_t ticks)
{
    timer->ticks += ticks;
    while (timer->ticks >= DIV_TICKS)
    {
        timer->ticks -= DIV_TICKS;
        mem->div++;
    }

    if (mem->tac & TAC_ENABLED)
    {
        uint16_t frequency = TAC_FREQUENCIES[mem->tac & 0x03];

        timer->tima_counter += ticks;
        while (timer->tima_counter >= frequency)
        {
            timer->tima_counter -= frequency;
            mem->tima++;
            if (mem->tima == 0x00) {
                mem->tima = mem->tma;
                request_interrupt(mem, TIMER_INTERRUPT);
            }
        }
    }
}