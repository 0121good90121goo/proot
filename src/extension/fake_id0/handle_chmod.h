#ifndef HANDLE_CHMOD_H_
#define HANDLE_CHMOD_H_

#include "shared_structs.h"

extern int handle_chmod(Tracee *tracee, Reg path_sysarg, Reg mode_sysarg, Reg fd_sysarg, Reg dirfd_sysarg, Config *config);

#endif
