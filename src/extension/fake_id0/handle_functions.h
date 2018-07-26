#ifndef HANDLE_FUNCTIONS_H_
#define

#include "shared_structs.h"

int handle_open(Tracee *tracee, Reg fd_sysarg, Reg path_sysarg, 
Reg flags_sysarg, Reg mode_sysarg, Config *config);

#endif
