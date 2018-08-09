#ifndef HANDLE_SYMLINK_H_
#define HANDLE_SYMLINK_H_

#include "shared_structs.h"

extern int handle_symlink(Tracee *tracee, Reg oldpath_sysarg, Reg newdirfd_sysarg, Reg newpath_sysarg, Config *config);

#endif
