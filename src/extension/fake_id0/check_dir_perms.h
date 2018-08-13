#ifndef CHECK_DIR_PERMS_H_
#define CHECK_DIR_PERMS_H_

#include <linux/limits.h>

#include "shared_structs.h"

extern int check_dir_perms(Tracee *tracee, char type, char path[PATH_MAX], 
    char rel_path[PATH_MAX], Config *config);

#endif
