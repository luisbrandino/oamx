#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <stdint.h>
#include "cpu.h"

typedef enum PcMode {
    PC_ADVANCE,
    PC_MANUAL
} PcMode;

typedef struct Instruction {
    char* name;
    uint8_t operand_length;
    uint8_t ticks;
    PcMode pc_mode;

    void (*handle)(Cpu* cpu, Memory* mem);
} Instruction;

uint8_t execute_instruction(Cpu* cpu, Memory* mem, uint8_t byte);

#endif