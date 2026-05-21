#include "init.h"
#include "load_rom.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int init(int argc, char *argv[]) {
    InitAction *action = init_get_action(argc, argv);
    if (action->type == NULL_ACTION) {
        printf("Error: Invalid command line arguments\n");
        return 1;
    }
    if (init_run_action(action) != 0) {
        printf("Error: Failed to run action\n");
    }

    free(action->s_param);
    free(action);

    return 0;
}

InitAction* init_get_action(int argc, char *argv[]) {
    InitAction *action = malloc(sizeof(InitAction));
    action->type = NULL_ACTION;

    if (strcmp(argv[1], "-o") == 0) {
        // -o <rom_file>
        if (argc == 3) {
            char *rom_file = argv[2];
            action->type = LOAD_ROM_FILE;
            action->s_param = rom_file;
        } else {
            printf("Usage: %s -o <rom_file>\n", argv[0]);
        }
    } else if (strcmp(argv[1], "-t") == 0) {
        // -t <rom_file_number>
        if (argc == 3) {
            int rom_file_number = atoi(argv[2]);
            action->type = LOAD_ROM_NUMBER;
            action->i_param = rom_file_number;
        } else {
            printf("Usage: %s -t <rom_file_number>\n", argv[0]);
        }
    }

    return action;
}

int init_run_action(InitAction *action) {
    switch (action->type) {
        case LOAD_ROM_FILE:
            printf("Loading ROM file: %s\n", action->s_param);
            load_rom_file(action->s_param);
            break;
        case LOAD_ROM_NUMBER:
            printf("Loading ROM file number: %d\n", action->i_param);
            load_rom_number(action->i_param);
            break;
        case NULL_ACTION:
            return 1;
    }

    return 0;
}