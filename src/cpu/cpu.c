#include "cpu.h"
#include "flags.h"
#include <stdlib.h>

CPU cpu_create(void) {
    Flags* flags = (Flags*) malloc(sizeof(Flags));
    *flags = flags_init();

    // CPUStack* stack = (CPUStack*) malloc(sizeof(CPUStack));
    // *stack = cpu_stack_init();

    return (CPU){
        .memory = NULL,
        .cycle = 0,
        .extraCycles = 0,

        .a = 0,
        .x = 0,
        .y = 0,
        .pc = 0,
        .sp = 0,
        .p = 0,
        .flags = flags,
        .d = 0,

        .db = 0,
        .pb = 0,

        .stack = NULL,
    };
}

void cpu_destroy(CPU *cpu) {
    if (cpu->flags != NULL) {
        free(cpu->flags);
        cpu->flags = NULL;
    }

    if (cpu->stack != NULL) {
        free(cpu->stack);
        cpu->stack = NULL;
    }

    free(cpu);
    cpu = NULL;
}