#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cartridge.h"
#include "utils.h"

// #define PRINT_HEADER_DETAILS

Cartridge* create_cartridge(FILE *file) {
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    int headered = is_rom_headered(size);
    if (headered == -1) {
        printf("Error: Invalid ROM size.\n");
        return NULL;
    }

    char *binary_string = int_to_binary((unsigned int)size, /* with_spaces = */ true);
    printf("ROM size: %ld, 0x%06X, 0b%s.\n", size, (unsigned int)size, binary_string);
    free(binary_string);
    binary_string = NULL;

    #ifdef PRINT_HEADER_DETAILS
    printf("ROM first 512 bytes.\n");
    print_512_bytes_from_file(file);
    #endif

    if (headered == 1) {
        // Remove header from the ROM
        size -= 512;
        // Set file pointer to skip the header
        fseek(file, 512L, SEEK_SET);

        binary_string = int_to_binary((unsigned int)size, /* with_spaces = */ true);
        printf("New ROM size: %ld, 0x%06X, 0b%s.\n", size, (unsigned int)size, binary_string);
        free(binary_string);
        binary_string = NULL;

        #ifdef PRINT_HEADER_DETAILS
        printf("New first 512 bytes of the ROM after removing the header:\n");
        print_512_bytes_from_file(file);
        #endif
    }

    // Figure out the target size of the ROM.
    long next_po2 = (long)next_power_of_2((long)size);
    long first_half_size = next_po2 / 2;
    long second_half_size = size - first_half_size;
    long target_second_half_size = next_power_of_2(second_half_size);
    long target_size = first_half_size + target_second_half_size;
    long remainder = target_size - size;

    uint8_t *bytes = (uint8_t*) malloc(sizeof(uint8_t) * (size_t)target_size);
    fread(bytes, sizeof(uint8_t), (size_t)size, file);

    #ifdef PRINT_HEADER_DETAILS
    printf("ROM bytes after reading from file (first 512 bytes).\n");
    print_512_bytes_from_array(bytes, false);
    #endif

    // Pad the ROM with zeros to make the size of its 2nd half a power of 2.
    if (remainder > 0) {
        printf("Warning: 2nd half of the ROM is not a power of 2. Padding with %ld zeros.\n", remainder);
        memset(bytes + size, 0, (size_t)(remainder));
    }

    Cartridge *cartridge = (Cartridge*) malloc(sizeof(Cartridge));
    cartridge->bytes = bytes;
    cartridge->size = target_size;
    cartridge->first_half_size = first_half_size;
    cartridge->second_half_size = target_second_half_size;

    #ifdef PRINT_HEADER_DETAILS
    printf("Cartridge bytes after creation (inside the constructor) (first 512 bytes).\n");
    print_512_bytes_from_array(cartridge->bytes, false);
    #endif

    return cartridge;
}

void destroy_cartridge(Cartridge *cartridge) {
    free(cartridge->bytes);
    cartridge->bytes = NULL;

    free(cartridge);
    cartridge = NULL;
}

long next_power_of_2(long size) {
    // return (long)(1L << (unsigned int)ceil(log2((double)size)));

    long po2 = 1;
    while (po2 < size) {
        po2 <<= 1;
    }
    return po2;
}

int is_rom_headered(long size) {
    int modulo = size % 1024;

    if (modulo == 512) {
        printf("ROM is headered.\n");
        return 1;
    } else if (modulo == 0) {
        printf("ROM is unheadered.\n");
        return 0;
    }

    // invalid ROM size
    printf("Error: Unable to determine if ROM is headered or unheadered. Invalid ROM size.\n");
    return -1;
}

void assemble_header(Cartridge *cartridge) {
    unsigned int header_location = locate_header(cartridge);

    printf("Header location: $%06X.\n", header_location);
}

const unsigned int candidate_header_locations[] = {
    0x007F00, // LoROM
    0x00FF00, // HiROM
    0x40FF00, // ExHiROM
};

void compute_checksum(Cartridge *cartridge, Checksum *checksum) {
    checksum->value = 0x0000;
    checksum->complement = 0x0000;

    // print_512_bytes_from_array(cartridge->bytes);

    for (int i = 0; i < cartridge->size; i++) {
        checksum->value = (uint16_t)(checksum->value + (uint16_t)cartridge->bytes[i]);
    }

    printf("Checksum of size: %04X.\n", checksum->value);

    // If the ROM size is not a power of 2, sum the second half of the ROM again.
    if (cartridge->first_half_size != cartridge->second_half_size) {
        printf("Warning: The ROM size is not a power of 2. Summing the second half again.\n");

        // print_512_bytes_from_array(cartridge->bytes + cartridge->first_half_size);

        for (int i = (int)cartridge->first_half_size; i < cartridge->size; i++) {
            checksum->value = (uint16_t)(checksum->value + (uint16_t)cartridge->bytes[i]);
        }

        printf("Checksum of size + 2nd half again: %04X.\n", checksum->value);
    }

    checksum->complement = checksum->value ^ 0xFFFF;
}

unsigned int locate_header(Cartridge *cartridge) {
    Checksum *checksum = (Checksum*) malloc(sizeof(Checksum));
    compute_checksum(cartridge, checksum);

    printf("Checking header locations... Checksum: %04X, Complement: %04X.\n", checksum->value, checksum->complement);
    // printf("Checking header locations... Checksum: $%04X, Complement: $%04X.\n", checksum->value, checksum->complement);

    unsigned int header_location = 0;
    unsigned int candidate_header_locations_length = sizeof(candidate_header_locations) / sizeof(candidate_header_locations[0]);
    for (unsigned int i = 0; i < candidate_header_locations_length; i++) {
        unsigned int candidate_header_location = candidate_header_locations[i];
        if (is_header_location_valid(candidate_header_location, cartridge, checksum)) {
            header_location = candidate_header_location;
            break;
        }
    }

    if (header_location == 0) {
        printf("Error: Unable to locate header. No valid header location found.\n");
        exit(1);
    }

    return header_location;
}

bool is_header_location_valid(unsigned int location, Cartridge *cartridge, Checksum *checksum) {
    // Checksum:   $xxxxDE
    // Complement: $xxxxDC
    unsigned int header_checksum_location = location + 0xDE;
    unsigned int header_complement_location = location + 0xDC;

    uint16_t header_checksum = make_uint16(cartridge->bytes[header_checksum_location + 1], cartridge->bytes[header_checksum_location]);
    uint16_t header_complement = make_uint16(cartridge->bytes[header_complement_location + 1], cartridge->bytes[header_complement_location]);

    if (header_checksum != checksum->value || header_complement != checksum->complement) {
        printf("Invalid header location: $%06X. Header Checksum: $%04X, Found complement: $%04X.\n", header_checksum_location, header_checksum, header_complement);
        return false;
    }

    printf("Valid header location found: $%06X.\n", location);

    return true;
}
