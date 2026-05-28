#include "rom.h"

char* resolve_predefined_rom_file_path(int rom_file_number);
ROM* rom_load_file(char *rom_file_path);
ROM* rom_load_number(int rom_file_number);
void rom_destroy(ROM *rom);
ROMHeaderStatus get_rom_header_status(unsigned int rom_file_size);
