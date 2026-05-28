#pragma once

#include <stdio.h>

typedef enum ROMHeaderStatus {
    ROM_HEADERED_NO,
    ROM_HEADERED_YES,
    ROM_HEADERED_UNKNOWN,
} ROMHeaderStatus;

typedef struct ROM {
    FILE *file;
    char *path;
    unsigned int size;
    ROMHeaderStatus header_status;
} ROM;
