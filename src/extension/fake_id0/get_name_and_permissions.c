
/** Gets the final component of a path.
 */

#include <linux/limits.h>
#include <string.h>

#include "shared_structs.h"
#include "dtoo.h"

#define OWNER_PERMS     0
#define GROUP_PERMS     1
#define OTHER_PERMS     2

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

/** Returns the mode pertinent to the level of permissions the user has. Eg if
 *  uid 1000 tries to access a file it owns with mode 751, this returns 7.
 */

int get_permissions(char meta_path[PATH_MAX], Config *config, bool uses_real)
{
    int perms;
    int omode;
    mode_t mode;
    uid_t owner, emulated_uid;
    gid_t group, emulated_gid;

    int status = read_meta_file(meta_path, &mode, &owner, &group, config);
    if(status < 0)
        return status;

    if(uses_real) {
        emulated_uid = config->ruid;
        emulated_gid = config->rgid;
    }
    else
        emulated_uid = config->euid;
        emulated_gid = config->egid;

    if (emulated_uid == owner || emulated_uid == 0)
        perms = OWNER_PERMS;
    else if(emulated_gid == group)
        perms = GROUP_PERMS;
    else
        perms = OTHER_PERMS;

    omode = dtoo(mode);
    switch(perms) {
    case OWNER_PERMS:
        omode /= 10;
    case GROUP_PERMS:
        omode /= 10;
    case OTHER_PERMS:
        omode = omode % 10;
    }

    /** Root always has RW permissions for every file. Has weird interactions
     *  with sudo v su, EG su can echo into a file with perms of 400 but sudo cannot.
     */
    if(emulated_uid == 0)
        omode |= 6;
    return omode;
}
