/** Gets the final component of a path.
 */

#include <linux/limits.h>
#include <string.h>

char * get_name(char path[PATH_MAX])
{
    char *name;

    name = strrchr(path,'/');
    if (name == NULL)
        name = path;
    else
        name++;

    return name;
}
