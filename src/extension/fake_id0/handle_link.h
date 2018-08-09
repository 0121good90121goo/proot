#ifndef HANDLE_LINK_H_
#define HANDLE_LINK_H_

#include "shared_structs.h"

extern int handle_link(Tracee *tracee, Reg olddirfd_sysarg, Reg oldpath_sysarg, Reg newdirfd_sysarg, Reg newpath_sysarg, Config *config);

#endif
