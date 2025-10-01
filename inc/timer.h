#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include "memory.h"

#define DIV_TICKS 256
#define TAC_ENABLED (1 << 2)

typedef struct {
    uint16_t ticks;
    uint16_t tima_counter;
} Timer;

void timer_update(Timer* timer, Memory* mem, uint16_t ticks);

#endif