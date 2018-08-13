#ifndef GET_META_PATH_H_
#define GET_META_PATH_H_

#include <linux/limits.h>

#include "shared_structs.h"

extern int get_meta_path(char orig_path[PATH_MAX], char meta_path[PATH_MAX]);

#endif
