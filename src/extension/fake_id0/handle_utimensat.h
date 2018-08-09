#ifndef HANDLE_UTIMENSAT_H_
#define HANDLE_UTIMENSAT_H_

#include "shared_structs.h"

extern int handle_utimensat(Tracee *tracee, Reg dirfd_sysarg, Reg path_sysarg, Reg times_sysarg, Config *config);

#endif
