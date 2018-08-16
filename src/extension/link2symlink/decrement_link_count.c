/* If path points a file that is a symlink to a file that begins
 *   with PREFIX, let the file be deleted, but also delete the 
 *   symlink that was created and decremnt the count that is tacked
 *   to end of original file.
 */

#include <linux/limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "tracee/reg.h"
#include "tracee/mem.h"
#include "extension/extension.h"
#include "path/path.h"

#include "my_readlink.h"

#define PREFIX ".proot.l2s."

int decrement_link_count(Tracee *tracee, Reg sysarg)
{
    char original[PATH_MAX];
    char intermediate[PATH_MAX];
    char final[PATH_MAX];
    char new_final[PATH_MAX];
    char * name;
    struct stat statl;
    ssize_t size;
    int status;
    int link_count;

    /* Note: this path was already canonicalized.  */
    size = read_string(tracee, original, peek_reg(tracee, CURRENT, sysarg), PATH_MAX);
    if (size < 0)
        return size;
    if (size >= PATH_MAX)
        return -ENAMETOOLONG;

    if(!belongs_to_guestfs(tracee, original))
        return 0;

    /* Check if it is a converted link already.  */
    status = lstat(original, &statl);
    if (status < 0)
        return 0;

    if (!S_ISLNK(statl.st_mode)) 
        return 0;

    status = my_readlink(original, intermediate);
    if (status < 0)
        return status;

    name = strrchr(intermediate, '/');
    if (name == NULL)
        name = intermediate;
    else
        name++;

    /* Check if an l2s file is pointed to */
    if (strncmp(name, PREFIX, strlen(PREFIX)) != 0) 
        return 0;

    status = my_readlink(intermediate, final);
    if (status < 0)
        return status;

    link_count = atoi(final + strlen(final) - 4);
    link_count--;
            
    /* Check if it is or is not the last link to delete */
    if (link_count > 0) {
        strncpy(new_final, final, strlen(final) - 4);
        sprintf(new_final + strlen(final) - 4, "%04d", link_count);     
    
        status = rename(final, new_final);
        if (status < 0)
            return status;              
        status = notify_extensions(tracee, LINK2SYMLINK_RENAME, (intptr_t) final, (intptr_t) new_final);
        if (status < 0)
            return status;
            
        strcpy(final, new_final);

        /* Symlink the intermediate to the final file.  */
        status = unlink(intermediate);
        if (status < 0)
            return status;

        status = symlink(final, intermediate);
        if (status < 0)
            return status;              
    } else {
        /* If it is the last, delete the intermediate and final */
        status = unlink(intermediate);
        if (status < 0)
            return status;
        status = unlink(final);
        if (status < 0)
            return status;
        status = notify_extensions(tracee, LINK2SYMLINK_UNLINK, (intptr_t) final, 0);
        if (status < 0)
            return status;
    }

    return 0;
}
