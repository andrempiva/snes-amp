#include "../munit/munit.h"

#include "rom_loader_tests.c"
#include "cartridge_tests.c"

int main(int argc, char *argv[]) {

    MunitTest tests[] = {
        {"/test_load_headered_rom", test_load_headered_rom, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
        {"/test_load_unheadered_rom", test_load_unheadered_rom, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
        {"/test_create_cartridge", test_create_cartridge, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
        {"/test_find_header_location", test_find_header_location, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
        { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
    };

    // MunitSuite rom_loader_suite = {"rom_loader", rom_loader_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE};

    MunitTest cartridge_tests[] = {

        // { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
    };

    // MunitSuite cartridge_suite = {"cartridge", cartridge_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE};

    MunitSuite suite = {"tests", tests, NULL, 1, MUNIT_SUITE_OPTION_NONE};

    int result = 0;

    // result += munit_suite_main(&rom_loader_suite, NULL, argc, argv);
    // result += munit_suite_main(&cartridge_suite, NULL, argc, argv);

    result += munit_suite_main(&suite, NULL, argc, argv);

    return result;
}
