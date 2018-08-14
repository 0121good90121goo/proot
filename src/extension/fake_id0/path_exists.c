/** Determines whether the file specified by path exists.
 */

#include <unistd.h>
#include <linux/limits.h>
#include <fcntl.h>

int path_exists(char path[PATH_MAX])
{
    return access(path, F_OK);  
}
