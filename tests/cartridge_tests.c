#include "../munit/munit.h"
#include "../src/cartridge.h"
#include "../src/rom_loader.h"

static MunitResult
test_create_cartridge(const MunitParameter params[], void *data) {
    (void) params;
    (void) data;

    ROM *rom = rom_load_number(0);

    Cartridge *cartridge = create_cartridge(rom);

    munit_assert_int(cartridge->size % 512, ==, 0);

    destroy_cartridge(cartridge);
    rom_destroy(rom);

    return MUNIT_OK;
}

static MunitResult
test_find_header_location(const MunitParameter params[], void *data) {
    (void) params;
    (void) data;

    ROM *rom = rom_load_number(0);

    Cartridge *cartridge = create_cartridge(rom);
    assemble_header(cartridge);

    munit_assert_int(cartridge->header_offset, !=, 0);

    destroy_cartridge(cartridge);
    rom_destroy(rom);

    return MUNIT_OK;
}
