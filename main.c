#include "init.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("Usage: %s [-o <rom_file>] [-t <rom_file_number>]\n", argv[0]);
        return 1;
    }

    int action_result = init(argc, argv);

    return action_result;
}
