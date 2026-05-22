#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cartridge.h"
#include "init.h"
#include "rom_loader.h"

int init(int argc, char *argv[]) {
    int return_value = 0;

    InitAction *action = init_action_create(argc, argv);
    if (action->type == ACTION_NULL) {
        printf("Error: Invalid command line arguments\n");
        exit(1);
    }

    switch (action->type) {
        case ACTION_LOAD_ROM_FILE:
        case ACTION_LOAD_ROM_NUMBER:
            return init_load_rom(action);

        // Other Actions...
        // ...

        // No action.
        case ACTION_NULL: return 1;

        default:
            printf("Error: Invalid action type: %d\n", action->type);
            return_value = -1;
    }

    // Clean up
    free(action);
    action = NULL;

    return return_value;
}

InitAction* init_action_create(int argc, char *argv[]) {
    InitAction *action = malloc(sizeof(InitAction));
    action->type = ACTION_NULL;

    if (strcmp(argv[1], "-o") == 0) {
        // -o <rom_file_path>
        if (argc == 3) {
            char *rom_file_path = argv[2];
            action->type = ACTION_LOAD_ROM_FILE;
            action->s_param = rom_file_path;
        } else {
            printf("Usage: %s -o <rom_file_path>\n", argv[0]);
        }
    } else if (strcmp(argv[1], "-t") == 0) {
        // -t <rom_file_number>
        if (argc == 3) {
            int rom_file_number = atoi(argv[2]);
            action->type = ACTION_LOAD_ROM_NUMBER;
            action->i_param = rom_file_number;
        } else {
            printf("Usage: %s -t <rom_file_number>\n", argv[0]);
        }
    }

    return action;
}

int init_load_rom(InitAction *action) {
    FILE* file = NULL;

    if (action->type == ACTION_LOAD_ROM_FILE) {
        file = load_rom_file(action->s_param);
    } else if (action->type == ACTION_LOAD_ROM_NUMBER) {
        file = load_rom_number(action->i_param);
    } else {
        printf("Error: Undefined action type on loading ROM: %d.\n", action->type);
        exit(1);
    }

    if (file == NULL) {
        printf("Error: Failed to load ROM file.\n");
        exit(1);
    }

    Cartridge *cartridge = create_cartridge(file);
    if (cartridge == NULL) {
        printf("Error: Failed to create cartridge.\n");
        exit(1);
    }

    // Clean up
    fclose(file);

    // <in_development>
    printf("ROM loaded successfully.\n");
    printf("Cartridge created successfully.\n");
    printf("Nothing to do at this point. Exiting...\n\n");
    printf("In development.\n");

    // Clean up
    destroy_cartridge(cartridge);
    // </in_development>

    return 0;
}
