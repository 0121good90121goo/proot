/**
 * Copy the contents of the @symlink into @value (nul terminated).
 * This function returns -errno if an error occured, otherwise 0.
 */

#include <unistd.h>
#include <errno.h>
#include <linux/limits.h>

int my_readlink(const char symlink[PATH_MAX], char value[PATH_MAX])
{
    ssize_t size;

    size = readlink(symlink, value, PATH_MAX);
    if (size < 0)
        return size;
    if (size >= PATH_MAX)
        return -ENAMETOOLONG;
    value[size] = '\0';

    return 0;
}
