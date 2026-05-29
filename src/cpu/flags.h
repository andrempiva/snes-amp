#pragma once

#include <stdint.h>

#include "utils.h"

// ## Flags
// 0    C     Carry         (0=No Carry, 1=Carry)
// 1    Z     Zero          (0=Nonzero, 1=Zero)
// 2    I     IRQ Disable   (0=IRQ Enable, 1=IRQ Disable)
// 3    D     Decimal Mode  (0=Normal, 1=BCD Mode for ADC/SBC opcodes)
// 4    X/B   Break Flag    (0=IRQ/NMI, 1=BRK/PHP opcode)  (0=16bit, 1=8bit)
// 5    M/U   Unused        (Always 1)                     (0=16bit, 1=8bit)
// 6    V     Overflow      (0=No Overflow, 1=Overflow)
// 7    N     Negative/Sign (0=Positive, 1=Negative)
// -    E                                                  (0=16bit, 1=8bit)

// https://problemkaputt.de/fullsnes.htm#cpuregistersandflags


// typedef struct Flags {
//     uint8_t c;
//     uint8_t z;
//     uint8_t i;
//     uint8_t d;
//     uint8_t xb;
//     uint8_t mu;
//     uint8_t v;
//     uint8_t n;
//     uint8_t e;
// } Flags;


typedef struct Flags Flags;
typedef struct Flags {
    bool c;
    bool z;
    bool i;
    bool d;
    bool xb;
    bool mu;
    bool v;
    bool n;

    bool e;
} Flags;

Flags flags_init(void);
void flags_set(Flags *flags, uint8_t value);
uint8_t flags_get(Flags *flags);
void flags_check_zero_negative(Flags *flags, uint8_t value);
void flags_print_details(Flags *flags);
