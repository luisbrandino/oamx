#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#define VBLANK_ADDR     0x0040
#define LCD_STAT_ADDR   0x0048
#define TIMER_ADDR      0x0050
#define SERIAL_ADDR     0x0058
#define JOYPAD_ADDR     0x0060

#define VBLANK_INTERRUPT    (1 << 0)
#define LCD_STAT_INTERRUPT  (1 << 1)
#define TIMER_INTERRUPT     (1 << 2)
#define SERIAL_INTERRUPT    (1 << 3)
#define JOYPAD_INTERRUPT    (1 << 4)

#define IE_ADDR 0xFFFF
#define IF_ADDR 0xFF0F

#include "cpu.h"
#include "ppu.h"

void handle_interrupts(Cpu* cpu, Ppu* ppu, Memory* mem);
void request_interrupt(Memory* mem, uint8_t interrupt_bit);

#endif