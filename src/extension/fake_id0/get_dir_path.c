/** Gets a path without its final component.
 */

#include <linux/limits.h>
#include <string.h>

#include "tracee/reg.h"

int get_dir_path(char path[PATH_MAX], char dir_path[PATH_MAX])
{
    int offset;

    strcpy(dir_path, path);
    offset = strlen(dir_path) - 1;
    if (offset > 0) {
        /* Skip trailing path separators. */
        while (offset > 1 && dir_path[offset] == '/')
            offset--;

        /* Search for the previous path separator. */
        while (offset > 1 && dir_path[offset] != '/')
            offset--;

        /* Cut the end of the string before the last component. */
        dir_path[offset] = '\0';
    }
    return 0;   
}
