#include "structs.c"

int handle_open(Tracee *tracee, Reg fd_sysarg, Reg path_sysarg,       
    Reg flags_sysarg, Reg mode_sysarg, Config *config);

