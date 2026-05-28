#include "../munit/munit.h"
#include "../src/rom_loader.h"

static MunitResult
test_load_headered_rom(const MunitParameter params[], void *data) {
    (void) params;
    (void) data;

    char path[] = "../roms/F-Zero (J).smc";
    char *filepath = malloc(strlen(path) + 1);
    strcpy(filepath, path);

    ROM *rom = rom_load_file(filepath);

    munit_assert_int(rom->header_status, ==, ROM_HEADERED_YES);

    return MUNIT_OK;
}

static MunitResult
test_load_unheadered_rom(const MunitParameter params[], void *data) {
    (void) params;
    (void) data;

    char path[] = "../roms/Super Metroid (JU) [!].smc";
    char *filepath = malloc(strlen(path) + 1);
    strcpy(filepath, path);

    ROM *rom = rom_load_file(filepath);

    munit_assert_int(rom->header_status, ==, ROM_HEADERED_NO);

    return MUNIT_OK;
}
