#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stdio.h>
#include "vrg.h"

char* int_to_binary(unsigned int value, bool with_spaces);
#ifdef TEST_INT_TO_BINARY
int main(int argc, char *argv[]);
#endif

#define print_512_bytes_from_array(...)   vrg(print_512_bytes_from_array, __VA_ARGS__)
#define print_512_bytes_from_array1(bytes)    print_512_bytes_from_arrayX(bytes, true)
#define print_512_bytes_from_array2(bytes, space) print_512_bytes_from_arrayX(bytes, space)

void print_512_bytes_from_arrayX(uint8_t *bytes, bool space);
void print_512_bytes_from_file(FILE *file);

#endif

uint16_t make_uint16(uint8_t high_byte, uint8_t low_byte);
