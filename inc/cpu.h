#ifndef CPU_H
#define CPU_H

#include "memory.h"
#include <stdint.h>

#define FLAG_ZERO 0x80
#define FLAG_NEGATIVE 0x40
#define FLAG_HALFCARRY 0x20
#define FLAG_CARRY 0x10

#define SET_FLAG(flag) cpu->f |= flag
#define CLEAR_FLAG(flag) cpu->f &= ~(flag)
#define IS_FLAG_SET(flag) (cpu->f & (flag)) != 0

typedef enum {
    CPU_RUNNING,
    CPU_HALTED,
    CPU_STOPPED
} CpuState;

typedef struct Cpu {
    uint8_t a;
    uint8_t f;

    uint8_t b;
    uint8_t c;

    uint8_t d;
    uint8_t e;
    
    uint8_t h;
    uint8_t l;

    uint16_t sp;
    uint16_t pc;

    CpuState state;
    uint8_t current_ticks;
} Cpu;

static inline uint16_t get_af(Cpu* cpu) { return ((uint16_t)cpu->a << 8 | (cpu->f & 0xF0)); } // only the upper 4 bits of F are valid in AF
static inline void set_af(Cpu* cpu, uint16_t value) { cpu->a = value >> 8 & 0xFF; cpu->f = value & 0xF0; }

static inline uint16_t get_bc(Cpu* cpu) { return ((uint16_t)cpu->b << 8) | cpu->c; }
static inline void set_bc(Cpu* cpu, uint16_t value) { cpu->b = value >> 8 & 0xFF; cpu->c = value & 0xFF; }

static inline uint16_t get_de(Cpu* cpu) { return ((uint16_t)cpu->d << 8) | cpu->e; }
static inline void set_de(Cpu* cpu, uint16_t value) { cpu->d = value >> 8 & 0xFF; cpu->e = value & 0xFF; }

static inline uint16_t get_hl(Cpu* cpu) { return ((uint16_t)cpu->h << 8) | cpu->l; }
static inline void set_hl(Cpu* cpu, uint16_t value) { cpu->h = value >> 8 & 0xFF; cpu->l = value & 0xFF; }

Cpu* cpu_init();
void cpu_reset(Cpu* cpu);

void cpu_add_ticks(Cpu* cpu, uint8_t ticks);
void cpu_advance_pc(Cpu* cpu, uint8_t value);
void cpu_push(Cpu* cpu, Memory* mem, uint16_t value);
uint16_t cpu_pop(Cpu* cpu, Memory* mem);
void cpu_call(Cpu* cpu, Memory* mem, uint16_t addr);
void cpu_ret(Cpu* cpu, Memory* mem);
uint8_t cpu_step(Cpu* cpu, Memory* mem);

#endif