#pragma once

#include <stdint.h>

#include "cpu/cpu.h"

// ## CPU Memory Addressing
// ------------------------------------
// Opcode Addressing Modes
//   Enum Value         Name           Native   Nocash
//   IMPLIED            Implied        -        A,X,Y,S,P
//   IMMEDIATE          Immediate      #nn      nn
//   ZERO_PAGE          Zero Page      nn       [nn]
//   ZERO_PAGE_X        Zero Page,X    nn,X     [nn+X]
//   ZERO_PAGE_Y        Zero Page,Y    nn,Y     [nn+Y]
//   ABSOLUTE           Absolute       nnnn     [nnnn]
//   INDEXED_ABSOLUTE_X Absolute,X     nnnn,X   [nnnn+X]
//   INDEXED_ABSOLUTE_Y Absolute,Y     nnnn,Y   [nnnn+Y]
//   INDEXED_INDIRECT   (Indirect,X)   (nn,X)   [[nn+X]]
//   INDIRECT_INDEXED   (Indirect),Y   (nn),Y   [[nn]+Y]

typedef struct CPU CPU;

typedef enum AddressingModeType {
    IMPLIED = 0,
    IMMEDIATE = 1,
    ZERO_PAGE = 2,
    ZERO_PAGE_X = 3,
    ZERO_PAGE_Y = 4,
    ABSOLUTE = 5,
    INDEXED_ABSOLUTE_X = 6,
    INDEXED_ABSOLUTE_Y = 7,
    INDEXED_INDIRECT = 8,
    INDIRECT_INDEXED = 9,
} AddressingModeType;

typedef enum GetValueType {
    GET_VALUE_UNSUPPORTED,
    GET_VALUE_READ,
} GetValueType;

typedef struct GetAddressParams {
    CPU *cpu;
    uint16_t address; // either absolute, zero page, offset...
    bool has_page_cross_penalty;
} GetAddressParams;

// get_address: () {}
// get_address: (cpu, absoluteAddress, hasPageCrossPenalty) {}
// get_address: (cpu, absoluteAddress) {}
// get_address: (cpu, address) {}
// get_address: (cpu, offset, hasPageCrossPenalty) {}
// get_address: (cpu, zeroPageAddress, hasPageCrossPenalty) {}
// get_address: (cpu, zeroPageAddress) {}

// get_value_type: read
// read(cpu, argument, hasPageCrossPenalty) {}
typedef struct GetValueParams {
    CPU *cpu;
    uint16_t argument;
    bool has_page_cross_penalty;
} GetValueParams;

typedef struct AddressingMode {
    AddressingModeType id;
    uint8_t input_size;
    uint16_t (*get_address)(GetAddressParams *params);
    GetValueType get_value_type;
    uint8_t (*get_value)(GetValueParams *params);
} AddressingMode;

AddressingMode* addressing_mode_get(AddressingModeType id);
