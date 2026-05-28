// ## References
// - https://snes.nesdev.org/wiki/ROM_header

#include <stdint.h>

#include "rom.h"

typedef enum MapMode {
    MAP_MODE_LOROM = 0,
    MAP_MODE_HIROM = 1,
    MAP_MODE_EXHIROM = 5,
} MapMode;

typedef struct Cartridge {
    uint8_t *bytes;
    unsigned int size;
    unsigned int first_half_size;
    unsigned int second_half_size;
    unsigned int header_offset;
    char title[22];
    bool rom_speed;
    MapMode map_mode;
    uint8_t chipset;
    uint8_t rom_size;
    uint8_t ram_size;
    uint8_t country;
    uint8_t developer_id;
    uint8_t version;
    uint16_t checksum;
    uint16_t checksum_complement;
    uint8_t interrupt_vectors[32];
} Cartridge;


Cartridge* create_cartridge(ROM *rom);
void destroy_cartridge(Cartridge *cartridge);
unsigned int next_power_of_2(unsigned int size);

void assemble_header(Cartridge *cartridge);
void print_header_details(Cartridge *cart);



void locate_header(Cartridge *cartridge);

// ## Header Verification
//
// The primary way to verify a candidate header is to evaluate the checksum it contains. Some flash-carts appear to use only the checksum to distinguish LoROM from HiROM.
// If no valid checksum can be found (e.g. ROM-hacks or homebrews often omit it), additional heuristics may be used to estimate validity:
// - ROM checksum matches.
// - Checksum and complement sum to $FFFF.
// - Map mode matches header location.
// - Specified ROM size is not smaller than file size.
// - A reset vector < $8000 is invalid because it points outside of ROM.
// - The first instruction at a valid reset vector is likely to be: `sei, clc, sec, stz, jmp, jml`
// - The first instruction at a valid reset vector is unlikely to be: `brk, cop, stp, wdm, $FF (sbc long)`
// - ROM and RAM sizes are reasonable.
// - Game name field is ASCII characters only.

// ## Checksum
//
// The checksum is a 16-bit sum of all of the bytes in the ROM, potentially with some portions repeated. It is always computed as if the ROM is a power of 2 in size, as given by the ROM header.
// However, some SNES games have a ROM data size that is not a power of 2, e.g. a 3MB game might use a 2MB ROM and a 1MB ROM together. These will use mirroring to fill remaining space to reach the next largest power of 2.
//
// ### Computing the Checksum
//
// Once we have a ROM prepared with a power of 2 size equal to what the ROM header specified, we may compute its checksum.
// Because the ROM header will be part of the computed checksum, before computing the checksum we should first fill the header's checksum and complement values with $0000 and $FFFF. Any value plus its complement will produce the same result, so this ensures the resulting checksum matches the ROM even after the computed checksum is replaced in the header.
// Once ready:
//
// 1. Start with a 16-bit checksum = 0.
// 2. Add every byte from the prepared data to the checksum. (Overflow is discarded.)
// 3. Store the checksum in the ROM header ($FFDE or equivalent).
// 4. Store checksum ^ $FFFF in the ROM header ($FFDC).
void compute_checksum(Cartridge *cartridge);
bool is_header_location_valid(unsigned int location, Cartridge *cartridge);
