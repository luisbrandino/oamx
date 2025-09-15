#ifndef CB_INSTRUCTIONS_H
#define CB_INSTRUCTIONS_H

#include "instructions.h"

typedef struct CBInstruction {
    char* name;
    uint8_t ticks;
    void (*handle)(Cpu* cpu, Memory* mem);
} CBInstruction;

void execute_cb_instruction(Cpu* cpu, Memory* mem, uint8_t byte);

#endif