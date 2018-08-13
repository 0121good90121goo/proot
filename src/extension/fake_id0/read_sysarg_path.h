#ifndef READ_SYSARG_PATH_H_
#define READ_SYSARG_PATH_H_

#include <linux/limits.h>

#include "shared_structs.h"

extern int read_sysarg_path(Tracee *tracee, char path[PATH_MAX], Reg path_sysarg, RegVersion version);

#endif
