#include "cpu/addressing_modes.h"
#include <stdlib.h>

static AddressingMode addressing_modes[] = {
    [IMPLIED] = {.id = IMPLIED, .input_size = 0, .get_address = NULL, .get_value_type = GET_VALUE_UNSUPPORTED, .get_value = NULL},
};

AddressingMode* addressing_mode_get(AddressingModeType id) {
    return &addressing_modes[id];
}
