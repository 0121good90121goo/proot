#ifndef MY_READLINK_H_
#define MY_READLINK_H_

#include <linux/limits.h>

extern int my_readlink(const char symlink[PATH_MAX], char value[PATH_MAX]);

#endif
