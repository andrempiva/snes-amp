#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cartridge.h"
#include "rom_loader.h"
#include "utils.h"

// #define PRINT_HEADER_DETAILS

// Predefined ROM data
const char* predefined_roms[] = {
    "F-Zero (J).smc",
    "Final Fantasy IV (J).smc",
    "Super Mario World (U) [!].smc",
    "Zelda no Densetsu (V1.0) (J).smc",
    "Super Metroid (JU) [!].smc",
};

const int PREDEF_ROMS_AMOUNT = sizeof(predefined_roms) / sizeof(predefined_roms[0]);
const char PREDEF_ROMS_FOLDER[] = "../roms/";

char* resolve_predefined_rom_file_path(int rom_file_number) {
    printf("Loading ROM file number: %d\n", rom_file_number);

    if (rom_file_number < 0 || rom_file_number >= PREDEF_ROMS_AMOUNT) {
        printf("Error: Invalid ROM file number: %d\n", rom_file_number);
        printf("Must be between 0 and %d\n", PREDEF_ROMS_AMOUNT - 1);
        exit(1);
    }

    char *rom_file = (char*)predefined_roms[rom_file_number];
    char *rom_file_path = strdup(PREDEF_ROMS_FOLDER);
    strcat(rom_file_path, rom_file);

    return rom_file_path;
}

ROM* rom_load_file(char *rom_file_path) {
    printf("Loading ROM file: %s\n", rom_file_path);

    FILE *file = fopen(rom_file_path, "rb");

    if (file == NULL) {
        printf("Error: Failed to open ROM file: %s\n", rom_file_path);
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    unsigned int rom_file_size = (unsigned int)ftell(file);
    rewind(file);

    char *binary_string = int_to_binary(rom_file_size, /* with_spaces = */ true);
    printf("ROM size: %u, 0x%06X, 0b%s.\n", rom_file_size, rom_file_size, binary_string);
    free(binary_string);
    binary_string = NULL;

    #ifdef PRINT_HEADER_DETAILS
    printf("ROM first 512 bytes.\n");
    print_512_bytes_from_file(file);
    #endif

    ROMHeaderStatus header_status = get_rom_header_status(rom_file_size);
    if (header_status == ROM_HEADERED_UNKNOWN) {
        printf("Error: No support for unknown ROM header status at the moment. Exiting...\n");
        exit(1);
    }

    ROM *rom = (ROM*) malloc(sizeof(ROM));
    rom->file = file;
    rom->path = strdup(rom_file_path);
    rom->size = rom_file_size;
    rom->header_status = header_status;

    return rom;
}


/**
 *
 * @param rom_file_size
 * @return ROMHeaderStatus
 */
ROMHeaderStatus get_rom_header_status(unsigned int rom_file_size) {
    unsigned int modulo = rom_file_size % 1024;

    if (modulo == 512) {
        printf("ROM is headered.\n");
        return ROM_HEADERED_YES;
    } else if (modulo == 0) {
        printf("ROM is unheadered.\n");
        return ROM_HEADERED_NO;
    }

    // invalid ROM size
    printf("Warning: Unable to determine if ROM is headered or unheadered from ROM size.\n");
    return ROM_HEADERED_UNKNOWN;
}


ROM* rom_load_number(int rom_file_number) {
    char *rom_file_path = resolve_predefined_rom_file_path(rom_file_number);

    if (rom_file_path == NULL) {
        printf("Error: Failed to resolve ROM file path: %d\n", rom_file_number);
        exit(1);
    }

    ROM *rom = rom_load_file(rom_file_path);

    return rom;
}

void rom_destroy(ROM *rom) {
    if (rom->path != NULL) {
        free(rom->path);
        rom->path = NULL;
    }

    if (rom->file != NULL) {
        fclose(rom->file);
        rom->file = NULL;
    }

    free(rom);
    rom = NULL;
}
