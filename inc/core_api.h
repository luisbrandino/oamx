#ifndef CORE_API_H
#define CORE_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "memory.h"
#include "cpu.h"

typedef struct {
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

    uint8_t state;
    uint8_t ime;
} GB_cpu_snapshot_t;

typedef struct {
    uint8_t vram[0x2000];
} GB_memory_snapshot_t;


uint8_t GB_get_cpu_snapshot(GB_cpu_snapshot_t* cpu_snapshot_out);
uint8_t GB_get_memory_snapshot(GB_memory_snapshot_t* memory_snapshout_out);

//void GB_apply_cpu_state(GB_cpu_state_t cpu_state);
//void GB_apply_memory_state(GB_memory_state_t memory_state);

void GB_set_breakpoint(uint16_t addr);
void GB_unset_breakpoint(uint16_t addr);

void GB_pause(void);
void GB_resume(void);

void GB_sync_snapshot(Cpu* cpu, Memory* mem);
void GB_apply_pending_deltas(Cpu* cpu, Memory* mem);

#ifdef __cplusplus
}
#endif

#endif