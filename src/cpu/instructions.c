#include <stdlib.h>
#include <string.h>

#include "instructions.h"
#include "cpu/cpu.h"

static Instruction instructions[255] = {
    {.code = "TAY", .argument_type = ARGUMENT_TYPE_NONE, .run = run_TAY},
};

Instruction* instruction_get(char *code) {
    size_t code_length = strlen(code);
    if (!code_length) {
        fprintf(stderr, "Error: Invalid code string getting instruction.\n");
        exit(EXIT_FAILURE);
    }
    for (size_t i = 0; i < sizeof(instructions) / sizeof(instructions[0]); i++) {
        if (strcmp(instructions[i].code, code) == 0) {
            return &instructions[i];
        }
    }
    fprintf(stderr, "Error: Instruction not found: %s\n", code);
    exit(EXIT_FAILURE);
    return NULL;
}

void run_TAY(CPU *cpu, uint16_t argument) {
    (void)argument;

    cpu->y = cpu->a;
    flags_check_zero_negative(cpu->flags, cpu->y & 0x00FF);
}
