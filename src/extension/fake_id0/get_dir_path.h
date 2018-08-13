#ifndef GET_DIR_PATH_H_
#define GET_DIR_PATH_H_

#include <linux/limits.h>

#include "shared_structs.h"

extern int get_dir_path(char path[PATH_MAX], char dir_path[PATH_MAX]);

#endif
