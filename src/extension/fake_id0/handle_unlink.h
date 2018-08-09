#ifndef HANDLE_UNLINK_H_
#define HANDLE_UNLINK_H_

#include "shared_structs.h"

extern int handle_unlink(Tracee *tracee, Reg fd_sysarg, Reg path_sysarg, Config *config);

#endif
