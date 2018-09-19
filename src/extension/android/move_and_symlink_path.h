#ifndef MOVE_AND_SYMLINK_PATH_H_
#define MOVE_AND_SYMLINK_PATH_H_

#include "tracee/reg.h"

extern int move_and_symlink_path(Tracee *tracee, Reg sysarg, Reg sysarg2, Reg sysarg_out);

#endif
