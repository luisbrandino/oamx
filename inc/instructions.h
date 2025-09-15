#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <stdint.h>
#include "cpu.h"

typedef enum {
    OPERAND_NONE,
    OPERAND_BYTE,
    OPERAND_WORD
} OperandSize;

typedef enum {
    PC_ADVANCE,
    PC_MANUAL
} PcMode;

typedef struct Instruction Instruction;
typedef struct Instruction {
    char* name;
    uint8_t base_ticks;
    OperandSize operand_size;
    PcMode pc_mode;
    uint8_t operand;
    uint16_t operand16;

    void (*handle)(Instruction* instruction, Cpu* cpu, Memory* mem);
} Instruction;

void execute(Cpu* cpu, Memory* mem, uint8_t byte);

#endif