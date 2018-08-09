#ifndef HANDLE_ACCESS_H_
#define HANDLE_ACCESS_H_

#include "shared_structs.h"

extern int handle_access(Tracee *tracee, Reg path_sysarg, Reg mode_sysarg, Reg dirfd_sysarg, Config *config);

#endif
