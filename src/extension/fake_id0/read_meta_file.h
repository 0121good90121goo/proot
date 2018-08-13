#ifndef READ_META_FILE_H_
#define READ_META_FILE_H_

#include <linux/limits.h>

#include "shared_structs.h"

extern int read_meta_file(char path[PATH_MAX], mode_t *mode, uid_t *owner, gid_t *group, Config *config);

#endif
