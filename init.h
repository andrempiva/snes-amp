typedef enum {
    ACTION_NULL,
    ACTION_LOAD_ROM_FILE,
    ACTION_LOAD_ROM_NUMBER,
} InitActionType;

typedef struct {
    InitActionType type;
    // string parameter
    char *s_param;
    // integer parameter
    int i_param;
} InitAction;

int init(int argc, char *argv[]);

InitAction* init_action_create(int argc, char *argv[]);
int init_action_run(InitAction *action);

int init_load_rom(InitAction *action);
