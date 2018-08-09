#ifndef HANDLE_MK_H_
#define HANDLE_MK_H_

#include "shared_structs.h"

extern int handle_mk(Tracee *tracee, Reg fd_sysarg, Reg path_sysarg, Reg mode_sysarg, Config *config);

#endif
