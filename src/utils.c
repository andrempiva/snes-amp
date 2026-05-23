#include <stdlib.h>
#include <stdio.h>

#include "utils.h"

#ifndef TEST_INT_TO_BINARY
#define printf(...) if(0){}
#endif

char* int_to_binary(unsigned int value, bool with_spaces) {
    // int length = sizeof(unsigned int) * 8;
    int length = 32;

    // add spaces for readability
    int str_length = length + ((int)with_spaces * (int)(length / 8));

    printf("value: %u, length: %d, str_length: %d\n", value, length, str_length);
    char *binary = (char*) malloc((size_t)str_length);
    for (int i = 0; i < str_length; i++) binary[i] = '0' + (i%10);
    binary[str_length - 1] = '\0';

    printf("binary: %s\n", binary);

    // Added by spaces for readability
    int str_additional_index = 0;

    // Temp var
    int insert_index = 0;
    for (int i = 0; i < length; i++) {
        if (with_spaces && i % 8 == 0 && i != 0) {
            insert_index = str_length - 2 - i - str_additional_index;
            binary[insert_index] = (i % 16 == 0) ? ' ' : '_';

            printf("add _ \t%*s\n", insert_index+1, "v");
            printf("at %d\t%s\n", insert_index, binary);

            str_additional_index++;
        }
        insert_index = str_length - 2 - i - str_additional_index;
        char insert_char = (char)(((value >> i) & 1) + '0');
        binary[insert_index] = insert_char;

        printf("add %c\t%*s\n", insert_char, insert_index+1, "v");
        printf("at %d\t%s\n", insert_index, binary);
    }

    return binary;
}

#ifndef TEST_INT_TO_BINARY
#undef printf
#endif

#ifdef TEST_INT_TO_BINARY
int main(int argc, char *argv[]) {
    // test int_to_binary
    unsigned int value = 123456789;

    if (argc > 1) {
        value = (unsigned int)atoi(argv[1]);
    }

    char *binary = int_to_binary(value, /* with_spaces = */ true);
    printf("Binary: %s\n", binary);
    free(binary);
    binary = NULL;
    return 0;
}
#endif

void print_512_bytes_from_arrayX(uint8_t *bytes, bool space) {
    char *spacer = (space ? " " : "\0");
    int per_line = space ? 16 : 32;

    for (int i = 0; i < 512; i++) {
        if (bytes[i] == 0) {
            printf("\x1b[90m");
        } else {
            printf("\x1b[0m");
        }

        printf("%02X%s", bytes[i], (i % per_line == (per_line - 1)) ? "\n" : spacer);
    }
    printf("\x1b[0m\n");
}

void print_512_bytes_from_file(FILE *file) {
    uint8_t bytes[512];
    fread(bytes, sizeof(uint8_t), 512, file);
    fseek(file, -512L, SEEK_CUR);

    print_512_bytes_from_array(bytes, false);
}

uint16_t make_uint16(uint8_t high_byte, uint8_t low_byte) {
    return (uint16_t)((((uint16_t)high_byte) << 8) | ((uint16_t)low_byte));
}