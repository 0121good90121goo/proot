/** Handles rename and renameat syscalls. If a meta file matching the file to
 *  to be renamed exists, renames the meta file as well. See rename(2) for
 *  returned permission errors.
 */

#include <linux/limits.h>
#include <string.h>
#include <unistd.h>

#include "tracee/reg.h"

#include "shared_structs.h"

#include "read_sysarg_path.h"
#include "get_fd_path.h"
#include "path_exists.h"
#include "get_meta_path.h"

int handle_rename(Tracee *tracee, Reg oldfd_sysarg, Reg oldpath_sysarg, 
    Reg newfd_sysarg, Reg newpath_sysarg, Config *config)
{
    int status;
    uid_t uid;
    gid_t gid;
    mode_t mode;
    char oldpath[PATH_MAX];
    char newpath[PATH_MAX];
    char rel_oldpath[PATH_MAX];
    char rel_newpath[PATH_MAX];
    char meta_path[PATH_MAX];

    status = read_sysarg_path(tracee, oldpath, oldpath_sysarg, CURRENT); 
    if(status < 0)
        return status;
    if(status == 1)
        return 0;

    status = read_sysarg_path(tracee, newpath, newpath_sysarg, CURRENT); 
    if(status < 0)
        return status;
    if(status == 1)
        return 0;

    status = get_fd_path(tracee, rel_oldpath, oldfd_sysarg, CURRENT);
    if(status < 0)
        return status;

    status = get_fd_path(tracee, rel_newpath, newfd_sysarg, CURRENT);
    if(status < 0)
        return status;

    status = check_dir_perms(tracee, 'w', oldpath, rel_oldpath, config);
    if(status < 0)
        return status;

    status = check_dir_perms(tracee, 'w', newpath, rel_newpath, config);
    if(status < 0)
        return status;

    // If a meta file exists, "copy" it to the new path.
    status = get_meta_path(oldpath, meta_path);
    if(status < 0)
        return status;

    if(path_exists(meta_path) != 0)
        return 0;

    read_meta_file(meta_path, &mode, &uid, &gid, config);
    unlink(meta_path);
    
    strcpy(meta_path, "");
    status = get_meta_path(newpath, meta_path);
    if(status < 0)
        return status;

    return write_meta_file(meta_path, mode, uid, gid, 0, config); 
}
