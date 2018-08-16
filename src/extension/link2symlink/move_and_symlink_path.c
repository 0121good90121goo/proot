/**
 * Move the path pointed to by @tracee's @sysarg to a new location,
 * symlink the original path to this new one, make @tracee's @sysarg
 * point to the new location.  This function returns -errno if an
 * error occured, otherwise 0.
 */

#include <linux/limits.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>

#include "tracee/reg.h"
#include "tracee/mem.h"
#include "extension/extension.h"
#include "path/path.h"
#include "syscall/syscall.h"
#include "syscall/sysnum.h"

#include "my_readlink.h"

#define PREFIX ".proot.l2s."

int move_and_symlink_path(Tracee *tracee, Reg sysarg, Reg sysarg2, Reg sysarg_out)
{
    char original[PATH_MAX];
    char original_newpath[PATH_MAX];
    char intermediate[PATH_MAX];
    char new_intermediate[PATH_MAX];
    char final[PATH_MAX];
    char new_final[PATH_MAX];
    char * name;
    char nlinks[6];
    int status;
    int link_count = 0;
    int first_link = 1;
    int intermediate_suffix = 1;
    struct stat statl;
    ssize_t size;

    /* Note: this path was already canonicalized.  */
    size = read_string(tracee, original_newpath, peek_reg(tracee, CURRENT, sysarg2), PATH_MAX);
    if (size < 0)
        return size;
    if (size >= PATH_MAX)
        return -ENAMETOOLONG;
    /* If newpath already exists, return appropriate error. */
    if(access(original_newpath, F_OK) == 0)
        return -EEXIST;

    /* Note: this path was already canonicalized.  */
    size = read_string(tracee, original, peek_reg(tracee, CURRENT, sysarg), PATH_MAX);
    if (size < 0)
        return size;
    if (size >= PATH_MAX)
        return -ENAMETOOLONG;

    if(!belongs_to_guestfs(tracee, original))
        return 1;

    if(!belongs_to_guestfs(tracee, original_newpath))
        return 1;

    /* Sanity check: directories can't be linked.  */
    status = lstat(original, &statl);
    if (status < 0)
        return status;
    if (S_ISDIR(statl.st_mode))
        return -EPERM;

    /* Check if it is a symbolic link.  */
    if (S_ISLNK(statl.st_mode)) {
        /* get name */
        status = my_readlink(original, intermediate);
        if (status < 0)
            return status;

        name = strrchr(intermediate, '/');
        if (name == NULL)
            name = intermediate;
        else
            name++;

        if (strncmp(name, PREFIX, strlen(PREFIX)) == 0)
            first_link = 0;
    } else {
        /* compute new name */
        if (strlen(PREFIX) + strlen(original) + 5 >= PATH_MAX)
            return -ENAMETOOLONG;

        name = strrchr(original,'/');
        if (name == NULL)
            name = original;
        else
            name++;

        strncpy(intermediate, original, strlen(original) - strlen(name));
        intermediate[strlen(original) - strlen(name)] = '\0';
        strcat(intermediate, PREFIX);
        strcat(intermediate, name);
    }

    if (first_link) {
        /*Move the original content to the new path. */
        do {
            sprintf(new_intermediate, "%s%04d", intermediate, intermediate_suffix);     
            intermediate_suffix++;
        } while ((access(new_intermediate,F_OK) != -1) && (intermediate_suffix < 1000)); 
        strcpy(intermediate, new_intermediate);

        /** Check to see if the old path has links already. If it does, change
         *  them to symbolic links.
         */
        if((int) statl.st_nlink > 1) {
            //Find the directory in which files are being linked
            int offset;
            ino_t inode;
            char dir_path[PATH_MAX];
            char full_path[PATH_MAX];
            struct stat dir_stat;
            struct dirent *dir;
            DIR *d;

            strcpy(dir_path, original);
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
 
            /* Search the directory for files with the same inode number. */
            inode = statl.st_ino;
            d = opendir(dir_path);
            while((dir = readdir(d)) != NULL) {
                /* Canonicalize the directory name */
                sprintf(full_path, "%s/%s", dir_path, dir->d_name); 
                stat(full_path, &dir_stat);

                if(dir_stat.st_ino == inode && strcmp(full_path, original) != 0) {
                    /* Recreate the hard link as a symlink. */
                    unlink(full_path);
                    status = symlink(intermediate, full_path);
                    link_count++;
                }
            }
            closedir(d);
        }

        /** Format the final file correctly. Add zeros until the length of
         *  nlinks (without the terminating \0) is 4. 
         */   
        sprintf(nlinks, ".%04d", link_count+2);  
        strcpy(final, intermediate);
        strcat(final, nlinks);

        status = rename(original, final);
        if (status < 0)
            return status;
        status = notify_extensions(tracee, LINK2SYMLINK_RENAME, (intptr_t) original, (intptr_t) final);
        if (status < 0)
            return status;

        /* Symlink the intermediate to the final file.  */
        status = symlink(final, intermediate);
        if (status < 0)
            return status;

        /* Symlink the original path to the intermediate one.  */
        status = symlink(intermediate, original);
        if (status < 0)
            return status;
    } 
    
    else {
        /*Move the original content to new location, by incrementing count at end of path. */
        status = my_readlink(intermediate, final);
        if (status < 0)
            return status;

        link_count = atoi(final + strlen(final) - 4);
        link_count++;

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
    }
    
    status = set_sysarg_path(tracee, intermediate, sysarg_out);
    if (status < 0) 
        return status;

    return 0;
}
