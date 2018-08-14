/** Writes mode, owner, and group to the meta file specified by path. If 
 *  is_creat is set to true, the umask needs to be used since it would have
 *  been by a real system call.
 */

#include <linux/limits.h>
#include <errno.h>
#include <stdio.h>

#include "shared_structs.h"

#include "dtoo.h"

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
