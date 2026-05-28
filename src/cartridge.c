#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cartridge.h"
#include "utils.h"

// #define PRINT_HEADER_DETAILS

Cartridge* create_cartridge(ROM *rom) {
    FILE *file = rom->file;

    unsigned int cart_size = rom->size;
    if (rom->header_status == ROM_HEADERED_YES) {
        // Remove header from the ROM
        cart_size -= 512;
        // Set file pointer to skip the header
        fseek(file, 512L, SEEK_SET);

        // <print>
        char *binary_string = int_to_binary(cart_size, /* with_spaces = */ true);
        printf("New ROM size: %u, $%06X, 0b%s.\n", cart_size, cart_size, binary_string);
        free(binary_string);
        binary_string = NULL;
        // </print>

        #ifdef PRINT_HEADER_DETAILS
        printf("New first 512 bytes of the ROM after removing the header:\n");
        print_512_bytes_from_file(file);
        #endif
    }

    // Figure out the target size of the ROM.
    unsigned int next_po2 = (unsigned int)next_power_of_2((unsigned int)cart_size);
    unsigned int first_half_size = next_po2 / 2;
    unsigned int second_half_size = cart_size - first_half_size;
    unsigned int target_second_half_size = next_power_of_2(second_half_size);
    unsigned int target_size = first_half_size + target_second_half_size;
    unsigned int remainder = target_size - cart_size;

    uint8_t *bytes = (uint8_t*) malloc(sizeof(uint8_t) * (size_t)target_size);
    fread(bytes, sizeof(uint8_t), (size_t)cart_size, file);

    #ifdef PRINT_HEADER_DETAILS
    printf("ROM bytes after reading from file (first 512 bytes).\n");
    print_512_bytes_from_array(bytes, false);
    #endif

    // Pad the ROM with zeros to make the size of its 2nd half a power of 2.
    if (remainder > 0) {
        printf("Warning: 2nd half of the ROM is not a power of 2. Padding with %u zeros.\n", remainder);
        memset(bytes + cart_size, 0, (size_t)(remainder));
    }

    Cartridge *cartridge = (Cartridge*) malloc(sizeof(Cartridge));
    cartridge->bytes = bytes;
    cartridge->size = target_size;
    cartridge->first_half_size = first_half_size;
    cartridge->second_half_size = target_second_half_size;
    cartridge->header_offset = 0;

    assemble_header(cartridge);

    return cartridge;
}

void destroy_cartridge(Cartridge *cartridge) {
    free(cartridge->bytes);
    cartridge->bytes = NULL;

    free(cartridge);
    cartridge = NULL;
}

unsigned int next_power_of_2(unsigned int size) {
    // return 1U << (unsigned int) (ceil(log2((double)size)));

    unsigned int po2 = 1;
    while (po2 < size) {
        po2 <<= 1;
    }
    return po2;
}

void compute_checksum(Cartridge *cart) {
    uint16_t checksum = 0x0000;

    // print_512_bytes_from_array(cart->bytes);

    for (unsigned int i = 0; i < cart->size; i++) {
        checksum = (uint16_t)(checksum + (uint16_t)cart->bytes[i]);
    }

    printf("Checksum of size: $%04X.\n", checksum);

    // If the ROM size is not a power of 2, sum the second half of the ROM again.
    if (cart->first_half_size != cart->second_half_size) {
        printf("Warning: The ROM size is not a power of 2. Summing the second half again.\n");

        // print_512_bytes_from_array(cart->bytes + cart->first_half_size);

        for (unsigned int i = cart->first_half_size; i < cart->size; i++) {
            checksum = (uint16_t)(checksum + (uint16_t)cart->bytes[i]);
        }

        printf("Checksum of size + 2nd half again: $%04X.\n", checksum);
    }

    cart->checksum = checksum;
    cart->checksum_complement = checksum ^ 0xFFFF;
}

void locate_header(Cartridge *cartridge) {
    compute_checksum(cartridge);

    printf("Checking header locations... Checksum: $%04X, Complement: $%04X.\n", cartridge->checksum, cartridge->checksum_complement);

    const unsigned int candidate_header_locations[] = {
        0x007F00, // LoROM
        0x00FF00, // HiROM
        0x40FF00, // ExHiROM
    };

    // unsigned int candidate_header_locations_length = sizeof(candidate_header_locations) / sizeof(candidate_header_locations[0]);
    unsigned int candidate_header_locations_length = 3;

    unsigned int header_location = 0;
    for (unsigned int i = 0; i < candidate_header_locations_length; i++) {
        unsigned int candidate_header_location = candidate_header_locations[i];
        if (is_header_location_valid(candidate_header_location, cartridge)) {
            header_location = candidate_header_location;
            printf("Valid header location found: $%06X.\n", header_location);
            break;
        }
    }

    if (header_location == 0) {
        printf("Error: Unable to locate header. No valid header location found.\n");
        exit(1);
    }

    // Header title starts at $C0.
    cartridge->header_offset = header_location + 0xC0;
}

bool is_header_location_valid(unsigned int location, Cartridge *cartridge) {
    // Checksum:   $xxxxDE
    // Complement: $xxxxDC
    unsigned int header_checksum_location = location + 0xDE;
    unsigned int header_complement_location = location + 0xDC;

    uint16_t header_checksum = get_uint16(cartridge->bytes + header_checksum_location);
    uint16_t header_complement = get_uint16(cartridge->bytes + header_complement_location);

    if (header_checksum != cartridge->checksum || header_complement != cartridge->checksum_complement) {
        printf("Invalid header location: $%06X. Header Checksum: $%04X, Header Complement: $%04X.\n", header_checksum_location, header_checksum, header_complement);
        return false;
    }

    return true;
}

void assemble_header(Cartridge *cart) {
    locate_header(cart);


    strncpy(cart->title, (char*) (cart->bytes + cart->header_offset), 21);
    cart->title[21] = '\0';

    cart->rom_speed = (cart->bytes[cart->header_offset + 21] & 0b00010000) != 0;
    cart->map_mode = cart->bytes[cart->header_offset + 21] & 0x0F;
    cart->chipset = cart->bytes[cart->header_offset + 22];
    cart->rom_size = (cart->bytes[cart->header_offset + 23]);
    cart->ram_size = (cart->bytes[cart->header_offset + 24]);
    cart->country = cart->bytes[cart->header_offset + 25];
    cart->developer_id = cart->bytes[cart->header_offset + 26];
    cart->version = cart->bytes[cart->header_offset + 27];
    strncpy((char*) cart->interrupt_vectors, (char*) (cart->bytes + cart->header_offset + 28), 32);
}

void print_header_details(Cartridge *cart) {
    printf("Header Address: $%06X\n", cart->header_offset);
    printf("Title: 「%s」\n", cart->title);
    printf("ROM Speed: %s\n", cart->rom_speed ? "Fast" : "Slow");
    printf("Map Mode: %d\n", cart->map_mode);
    printf("Chipset: %d\n", cart->chipset);
    printf("ROM Size: %d (%d KB)\n", cart->rom_size, 1 << cart->rom_size);
    printf("RAM Size: %d (%d KB)\n", cart->ram_size, 1 << cart->ram_size);
    printf("Country: %d\n", cart->country);
    printf("Developer ID: %d\n", cart->developer_id);
    printf("ROM Version: %d\n", cart->version);
    printf("Checksum: $%04X\n", cart->checksum);
    printf("Checksum Complement: $%04X\n", cart->checksum_complement);
    printf("Interrupt Vectors:\n  ");
    for (unsigned int i = 0; i < 32; i++) {
        printf(" %02X", cart->interrupt_vectors[i]);
        if (i % 8 == 7) {
            printf("\n  ");
        }
    }
    printf("\n");
}
