/** Determines whether the file specified by path exists.
 */

#include <unistd.h>
#include <linux/limits.h>
#include <fcntl.h>

#include "tracee/reg.h"
#include "tracee/mem.h"
#include "path/path.h"

#define IGNORE_SYSARG (Reg)2000

int path_exists(char path[PATH_MAX])
{
    return access(path, F_OK);  
}

/** Gets a path from file descriptor system argument. If that sysarg is
 *  IGNORE_FLAGS, it returns the root of the guestfs, and if the file
 *  descriptor refers to the cwd, it returns that. Returning the root
 *  is used in cases where the function is used to find relative paths
 *  for __at calls.
 */

int get_fd_path(Tracee *tracee, char path[PATH_MAX], Reg fd_sysarg, RegVersion version)
{
    int status;

    if(fd_sysarg != IGNORE_SYSARG) {
        // AT_CWD translates to -100, so replace it with a canonicalized version
        if((signed int) peek_reg(tracee, version, fd_sysarg) == -100) 
            status = getcwd2(tracee, path);

        else {
            // See read_sysarg_path for an explanation of the use of modified.
            status = readlink_proc_pid_fd(tracee->pid, peek_reg(tracee, version, fd_sysarg), path);
        }
        if(status < 0) 
            return status;
    }

    else 
        translate_path(tracee, path, AT_FDCWD, "/", true);
    
    /** If a path does not belong to the guestfs, a handler either exits with 0
     *  or sets the syscall to void (in the case of chmod and chown.
     */
    if(!belongs_to_guestfs(tracee, path))
        return 1;

    return 0;
}

/** Reads a path from path_sysarg into path.
 */

int read_sysarg_path(Tracee *tracee, char path[PATH_MAX], Reg path_sysarg, RegVersion version)
{
    int size;
    char original[PATH_MAX];
    /** Current is already canonicalized. . Modified is used here
     *  for exit calls because on ARM architectures, the result to a system
     *  call is placed in SYSARG_1. Using MODIFIED allows the original path to
     *  be read. ORIGINAL is necessary in the case of execve(2) because of the
     *  modifications that PRoot makes to the path of the executable.
     */
    switch(version) {
        case MODIFIED:
            size = read_string(tracee, path, peek_reg(tracee, MODIFIED, path_sysarg), PATH_MAX);
            break;
        case CURRENT:
            size = read_string(tracee, path, peek_reg(tracee, CURRENT, path_sysarg), PATH_MAX);
            break;
        case ORIGINAL:
            size = read_string(tracee, original, peek_reg(tracee, ORIGINAL, path_sysarg), PATH_MAX);
            translate_path(tracee, path, AT_FDCWD, original, true);
            break;
        /* Never hit */
        default:
            size = 0;   //Shut the compiler up
            break;
    }

    if(size < 0)
        return size;
    if(size >= PATH_MAX)
        return -ENAMETOOLONG;

    /** If a path does not belong to the guestfs, a handler either exits with 0
     *  or sets the syscall to void (in the case of chmod and chown). Checking
     *  whether or not a path belongs to the guestfs only needs to happen if
     *  that path actually exists. Removing this check will cause some package
     *  installations to fail because they try to create symlinks with null
     *  targets.
     */
    if(strlen(path) > 0)
        if(!belongs_to_guestfs(tracee, path))
            return 1;

    return 0;
}
