#ifndef HANDLE_OPEN_H_
#define HANDLE_OPEN_H_

#include "shared_structs.h"

extern int handle_open(Tracee *tracee, Reg fd_sysarg, Reg path_sysarg, Reg flags_sysarg, Reg mode_sysarg, Config *config);

#endif
