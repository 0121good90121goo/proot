/** Stores in meta_path the contents of orig_path with the addition of META_TAG
 *  to the final component.
 */

#define META_TAG ".proot-meta-file."

#include <linux/limits.h>
#include <errno.h>
#include <string.h>

#include "get_dir_path.h"
#include "get_name.h"

int get_meta_path(char orig_path[PATH_MAX], char meta_path[PATH_MAX]) 
{
    char *filename;

    /*Separate the final component from the path. */
    get_dir_path(orig_path, meta_path);
    filename = get_name(orig_path);

    /* Add a / between the final component and the rest of the path. */
    if(strcmp(meta_path, "/") != 0)
        strcat(meta_path, "/");

    if(strlen(meta_path) + strlen(filename) + strlen(META_TAG) >= PATH_MAX)
        return -ENAMETOOLONG;

    /* Insert the meta_tag between the path and its final component. */
    strcat(meta_path, META_TAG);
    strcat(meta_path, filename);
    return 0;
}
