typedef enum {
    NULL_ACTION,
    LOAD_ROM_FILE,
    LOAD_ROM_NUMBER,
} InitActionType;

typedef struct {
    InitActionType type;
    char *s_param;
    int i_param;
} InitAction;

int init(int argc, char *argv[]);
InitAction* init_get_action(int argc, char *argv[]);
int init_run_action(InitAction *action);
