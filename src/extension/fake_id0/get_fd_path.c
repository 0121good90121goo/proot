/** Gets a path from file descriptor system argument. If that sysarg is 
 *  IGNORE_FLAGS, it returns the root of the guestfs, and if the file 
 *  descriptor refers to the cwd, it returns that. Returning the root
 *  is used in cases where the function is used to find relative paths
 *  for __at calls.
 */

#define IGNORE_SYSARG (Reg)2000

#include <linux/limits.h>

#include "tracee/reg.h"
#include "path/path.h"

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
