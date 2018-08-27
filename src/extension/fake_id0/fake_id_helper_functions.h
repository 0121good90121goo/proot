#ifndef FAKE_ID_HELPER_FUNCTIONS_H_
#define FAKE_ID_HELPER_FUNCTIONS_H_

#include <linux/limits.h>

#include "tracee/reg.h"
#include "shared_structs.h"

extern int check_dir_perms(Tracee *tracee, char type, char path[PATH_MAX], 
char rel_path[PATH_MAX], Config *config);

extern int get_dir_path(char path[PATH_MAX], char dir_path[PATH_MAX]);

extern int dtoo(int n);
extern int otod(int n);

extern int get_meta_path(char orig_path[PATH_MAX], char meta_path[PATH_MAX]);

extern int read_meta_file(char path[PATH_MAX], mode_t *mode, uid_t *owner, 
gid_t *group, Config *config);

extern int write_meta_file(char path[PATH_MAX], mode_t mode, uid_t owner, gid_t group,
bool is_creat, Config *config);

extern char * get_name(char path[PATH_MAX]);

extern int get_permissions(char meta_path[PATH_MAX], Config *config, bool uses_real);

extern int path_exists(char path[PATH_MAX]);

extern int get_fd_path(Tracee *tracee, char path[PATH_MAX], Reg fd_sysarg, RegVersion version);

extern int read_sysarg_path(Tracee *tracee, char path[PATH_MAX], Reg path_sysarg, RegVersion version);

#endif
