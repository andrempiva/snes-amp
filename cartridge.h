#include <stdio.h>
#include <stdint.h>

typedef struct {
    uint8_t *bytes;
    long size;
} Cartridge;

Cartridge* create_cartridge(FILE *file);
void destroy_cartridge(Cartridge *cartridge);
int is_rom_headered(long size);
