#include "cartridge.h"
#include <stdio.h>
#include <stdlib.h>

Cartridge* create_cartridge(FILE *file) {
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    int headered = is_rom_headered(size);
    if (headered == -1) {
        printf("Error: Invalid ROM size.\n");
        return NULL;
    }

    if (headered == 1) {
        size -= 512;
        fseek(file, 512, SEEK_SET);
    }

    // read ROM into Cartridge
    uint8_t *bytes = (uint8_t*) malloc(sizeof(uint8_t) * (size_t)size);
    fread(bytes, sizeof(uint8_t), (size_t)size, file);

    Cartridge *cartridge = (Cartridge*) malloc(sizeof(Cartridge));
    cartridge->bytes = bytes;
    cartridge->size = size;

    return cartridge;
}


void destroy_cartridge(Cartridge *cartridge) {
    free(cartridge->bytes);
    free(cartridge);
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
