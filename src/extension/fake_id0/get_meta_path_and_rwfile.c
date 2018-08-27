/** Stores in meta_path the contents of orig_path with the addition of META_TAG
 *  to the final component.
 */

#define META_TAG ".proot-meta-file."

#include <linux/limits.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

#include "shared_structs.h"

#include "fake_id_helper_functions.h"

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

/** Stores in mode, owner, and group the relative information found in the meta
 *  meta file. If the meta file doesn't exist, it reverts back to the original
 *  functionality of PRoot, with the addition of setting the mode to 755.
 */

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

/** Writes mode, owner, and group to the meta file specified by path. If 
 *  is_creat is set to true, the umask needs to be used since it would have
 *  been by a real system call.
 */

int write_meta_file(char path[PATH_MAX], mode_t mode, uid_t owner, gid_t group,
    bool is_creat, Config *config)
{
    FILE *fp;
    fp = fopen(path, "w");
    if(!fp)
        //Errno is set
        return -1;

    /** In syscalls that don't have the ability to create a file (chmod v open)
     *  for example, the umask isn't used in determining the permissions of the
     *  the file.
     */
    if(is_creat)
        mode = (mode & ~(config->umask) & 0777);

    fprintf(fp, "%d\n%d\n%d\n", dtoo(mode), owner, group);
    fclose(fp);
    return 0;
}
