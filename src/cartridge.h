// ## References
// - https://snes.nesdev.org/wiki/ROM_header

#include <stdint.h>
#include <stdio.h>

typedef struct {
    uint8_t *bytes;
    long size;
    long first_half_size;
    long second_half_size;
    long header_address;
} Cartridge;

Cartridge* create_cartridge(FILE *file);
void destroy_cartridge(Cartridge *cartridge);
long next_power_of_2(long size);
int is_rom_headered(long size);

void assemble_header(Cartridge *cartridge);



unsigned int locate_header(Cartridge *cartridge);

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

typedef struct {
    uint16_t value;
    uint16_t complement;
} Checksum;

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
void compute_checksum(Cartridge *cartridge, Checksum *checksum);
bool is_header_location_valid(unsigned int location, Cartridge *cartridge, Checksum *checksum);
