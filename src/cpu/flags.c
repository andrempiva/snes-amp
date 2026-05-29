#include <stdint.h>

#include "flags.h"
#include "utils.h"


Flags flags_init(void) {
    return (Flags){
        .c  = false,
        .z  = false,
        .i  = false,
        .d  = false,
        .xb = false,
        .mu = false,
        .v  = false,
        .n  = false,

        .e  = false,
    };
}

void flags_set(Flags *flags, uint8_t value) {
    flags->c  = is_bit_set(value, 0);
    flags->z  = is_bit_set(value, 1);
    flags->i  = is_bit_set(value, 2);
    flags->d  = is_bit_set(value, 3);
    flags->xb = is_bit_set(value, 4);
    flags->mu = is_bit_set(value, 5);
    flags->v  = is_bit_set(value, 6);
    flags->n  = is_bit_set(value, 7);
}

uint8_t flags_get(Flags *flags) {
    return (
        (flags->c  ? 1 << 0 : 0) |
        (flags->z  ? 1 << 1 : 0) |
        (flags->i  ? 1 << 2 : 0) |
        (flags->d  ? 1 << 3 : 0) |
        (flags->xb ? 1 << 4 : 0) |
        (flags->mu ? 1 << 5 : 0) |
        (flags->v  ? 1 << 6 : 0) |
        (flags->n  ? 1 << 7 : 0)) & 0xFF;
}

void flags_check_zero_negative(Flags *flags, uint8_t value) {
    flags->z = value == 0;
    flags->n = is_bit_set(value, 7);
}

void flags_print_details(Flags *flags) {
    printf("Flags:\n");
    printf("  c: %s\n", flags->c ? "Set" : "Clear");
    printf("  z: %s\n", flags->z ? "Set" : "Clear");
    printf("  i: %s\n", flags->i ? "Set" : "Clear");
    printf("  d: %s\n", flags->d ? "Set" : "Clear");
    printf("x/b: %s\n", flags->xb ? "Set" : "Clear");
    printf("m/u: %s\n", flags->mu ? "Set" : "Clear");
    printf("  v: %s\n", flags->v ? "Set" : "Clear");
    printf("  n: %s\n", flags->n ? "Set" : "Clear");
    printf("  e: %s\n", flags->e ? "Set" : "Clear");
}