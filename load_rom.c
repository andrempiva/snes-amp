#include "load_rom.h"
#include "cartridge.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* test_roms[] = {
    "F-Zero (J).smc",
    "Final Fantasy IV (J).smc",
    "Super Mario World (U) [!].smc",
    "Zelda no Densetsu (V1.0) (J).smc",
    "Super Metroid (JU) [!].smc",
};

const int TEST_ROM_LENGTH = sizeof(test_roms) / sizeof(test_roms[0]);
const char ROM_LIB_FOLDER[] = "./roms/";

int load_rom_file(char *rom_file_path) {
    printf("Loading ROM file: %s\n", rom_file_path);

    FILE *file = fopen(rom_file_path, "rb");

    if (file == NULL) {
        printf("Error: Failed to open ROM file: %s\n", rom_file_path);
        return 1;
    }

    Cartridge *cartridge = create_cartridge(file);
    if (cartridge == NULL) {
        printf("Error: Failed to create cartridge.\n");
        return 1;
    }

    fclose(file);

    // Do Whatever
    // Run game
    // Open emulator

    destroy_cartridge(cartridge);

    return 0;
}

int load_rom_number(int rom_file_number) {
    printf("Loading ROM file number: %d\n", rom_file_number);

    if (rom_file_number < 0 || rom_file_number >= TEST_ROM_LENGTH) {
        printf("Error: Invalid ROM file number: %d\n", rom_file_number);
        printf("Must be between 0 and %d\n", TEST_ROM_LENGTH - 1);
        return 1;
    }

    char *rom_file = (char*)test_roms[rom_file_number];
    char *rom_file_path = (char*) malloc(strlen(ROM_LIB_FOLDER) + strlen(rom_file) + 1);

    strcpy(rom_file_path, ROM_LIB_FOLDER);
    strcat(rom_file_path, rom_file);

    return load_rom_file(rom_file_path);
}
