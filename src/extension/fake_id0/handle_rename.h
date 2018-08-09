#ifndef HANDLE_RENAME_H_
#define HANDLE_RENAME_H_

#include "shared_structs.h"

extern int handle_rename(Tracee *tracee, Reg oldfd_sysarg, Reg oldpath_sysarg, Reg newfd_sysarg, Reg newpath_sysarg, Config *config);

#endif
