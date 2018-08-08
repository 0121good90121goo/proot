#ifndef HANDLE_FUNCTIONS_H_
#define HANDLE_FUNCTIONS_H_

#include "shared_structs.h"

extern int handle_open(Tracee *tracee, Reg fd_sysarg, Reg path_sysarg, Reg flags_sysarg, Reg mode_sysarg, Config *config);

extern int handle_mk(Tracee *tracee, Reg fd_sysarg, Reg path_sysarg, Reg mode_sysarg, Config *config);

extern int handle_unlink(Tracee *tracee, Reg fd_sysarg, Reg path_sysarg, Config *config);

extern int handle_rename(Tracee *tracee, Reg oldfd_sysarg, Reg oldpath_sysarg, Reg newfd_sysarg, Reg newpath_sysarg, Config *config);

extern int handle_chmod(Tracee *tracee, Reg path_sysarg, Reg mode_sysarg, Reg fd_sysarg, Reg dirfd_sysarg, Config *config);

extern int handle_chown(Tracee *tracee, Reg path_sysarg, Reg owner_sysarg, Reg group_sysarg, Reg fd_sysarg, Reg dirfd_sysarg, Config *config);

extern int handle_utimensat(Tracee *tracee, Reg dirfd_sysarg, Reg path_sysarg, Reg times_sysarg, Config *config);

extern int handle_access(Tracee *tracee, Reg path_sysarg, Reg mode_sysarg, Reg dirfd_sysarg, Config *config);

#endif
