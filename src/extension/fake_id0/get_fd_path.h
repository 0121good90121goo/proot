#ifndef GET_FD_PATH_H_
#define GET_FD_PATH_H_

#include <linux/limits.h>

#include "shared_structs.h"

extern int get_fd_path(Tracee *tracee, char path[PATH_MAX], Reg fd_sysarg, RegVersion version);

#endif
