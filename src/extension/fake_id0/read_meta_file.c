/** Stores in mode, owner, and group the relative information found in the meta
 *  meta file. If the meta file doesn't exist, it reverts back to the original
 *  functionality of PRoot, with the addition of setting the mode to 755.
 */

#include <linux/limits.h>
#include <stdio.h>

#include "shared_structs.h"

int read_meta_file(char path[PATH_MAX], mode_t *mode, uid_t *owner, gid_t *group, Config *config)
{
    FILE *fp;
    int lcl_mode;
    fp = fopen(path, "r");
    if(!fp) { 
        /* If the metafile doesn't exist, allow overly permissive behavior. */
        *owner = config->euid;
        *group = config->egid;
        *mode = otod(755);
        return 0;

    }
    fscanf(fp, "%d %d %d ", &lcl_mode, owner, group);
    lcl_mode = otod(lcl_mode);
    *mode = (mode_t)lcl_mode;
    fclose(fp);
    return 0;
}
