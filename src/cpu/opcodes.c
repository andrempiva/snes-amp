#include "opcodes.h"
#include "instructions.h"

#define INSTGET(id) instruction_get(id)

static Opcode opcodes_table[255];

void opcodes_init(void) {
    Opcode opcodes[] = {
        {.id = 0xA8, .instruction = INSTGET("TAY"), .cycles = 2, .addressing_mode = NULL},
        {.id = 0xAA, .instruction = NULL, .cycles = 2, .addressing_mode = NULL},
        {.id = 0xBA, .instruction = NULL, .cycles = 2, .addressing_mode = NULL},
        {.id = 0x98, .instruction = NULL, .cycles = 2, .addressing_mode = NULL},
        {.id = 0x8A, .instruction = NULL, .cycles = 2, .addressing_mode = NULL},
        {.id = 0x9A, .instruction = NULL, .cycles = 2, .addressing_mode = NULL},
        {.id = 0x9B, .instruction = NULL, .cycles = 2, .addressing_mode = NULL},
        {.id = 0xBB, .instruction = NULL, .cycles = 2, .addressing_mode = NULL},
        {.id = 0x7B, .instruction = NULL, .cycles = 2, .addressing_mode = NULL},
        {.id = 0x5B, .instruction = NULL, .cycles = 2, .addressing_mode = NULL},
        {.id = 0x3B, .instruction = NULL, .cycles = 2, .addressing_mode = NULL},
        {.id = 0x1B, .instruction = NULL, .cycles = 2, .addressing_mode = NULL},
    };

    for (size_t i = 0; i < sizeof(opcodes) / sizeof(opcodes[0]); i++) {
        opcodes_table[opcodes[i].id] = opcodes[i];
    }
}
