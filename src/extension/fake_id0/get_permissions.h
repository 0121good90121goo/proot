#ifndef GET_PERMISSIONS_H_
#define GET_PERMISSIONS_H_

#include <linux/limits.h>

#include "shared_structs.h"

extern int get_permissions(char meta_path[PATH_MAX], Config *config, bool uses_real);

#endif

