#pragma once

#include <stdint.h>

#include "cpu/instructions.h"
#include "cpu/addressing_modes.h"

typedef struct CPU CPU;
typedef struct AddressingMode AddressingMode;

typedef struct Opcode {
    uint8_t id;
    Instruction* instruction;
    uint8_t cycles;
    AddressingMode* addressing_mode;
} Opcode;
