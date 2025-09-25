#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include "memory.h"

#define DIV_TICKS 256
#define TAC_ENABLED (1 << 2)

#define DIV_ADDR 0xFF04
#define TIMA_ADDR 0xFF05
#define TMA_ADDR 0xFF06
#define TAC_ADDR 0xFF07

typedef struct {
    uint16_t div_counter;
    uint16_t tima_counter;
} Timer;

void timer_update(Timer* timer, Memory* mem, uint16_t ticks);

#endif