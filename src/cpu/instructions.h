#pragma once

#include <stdint.h>

#include "cpu/cpu.h"

typedef struct CPU CPU;

typedef enum ArgumentType {
    ARGUMENT_TYPE_NONE,
    ARGUMENT_TYPE_ADDRESS,
    ARGUMENT_TYPE_VALUE,
} ArgumentType;

typedef struct Instruction {
    char *code; // native instruction code (e.g. LDA, LDX, INC, etc.)
    ArgumentType argument_type;
    void (*run)(CPU *cpu, uint16_t argument);
} Instruction;

static Instruction instructions[255];

Instruction* instruction_get(char *code);

void run_TAY(CPU *cpu, uint16_t argument);