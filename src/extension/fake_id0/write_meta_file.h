#ifndef WRITE_META_FILE_H_
#define WRITE_META_FILE_H_

#include <linux/limits.h>

#include "shared_structs.h"

extern int write_meta_file(char path[PATH_MAX], mode_t mode, uid_t owner, gid_t group,
    bool is_creat, Config *config);

#endif
