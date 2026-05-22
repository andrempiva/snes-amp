#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cartridge.h"
#include "rom_loader.h"

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
    char *rom_file_path = (char*) malloc(strlen(PREDEF_ROMS_FOLDER) + strlen(rom_file) + 1);

    strcpy(rom_file_path, PREDEF_ROMS_FOLDER);
    strcat(rom_file_path, rom_file);

    return rom_file_path;
}

FILE* load_rom_file(char *rom_file_path) {
    printf("Loading ROM file: %s\n", rom_file_path);

    FILE *file = fopen(rom_file_path, "rb");

    if (file == NULL) {
        printf("Error: Failed to open ROM file: %s\n", rom_file_path);
        exit(1);
    }

    return file;
}

FILE* load_rom_number(int rom_file_number) {
    char *rom_file_path = resolve_predefined_rom_file_path(rom_file_number);

    if (rom_file_path == NULL) {
        printf("Error: Failed to resolve ROM file path: %d\n", rom_file_number);
        exit(1);
    }

    FILE *file = load_rom_file(rom_file_path);

    if (file == NULL) {
        printf("Error: Failed to load ROM file: %s\n", rom_file_path);
        exit(1);
    }

    free(rom_file_path);
    rom_file_path = NULL;

    return file;
}
