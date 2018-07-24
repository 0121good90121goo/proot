typedef struct {
    uid_t ruid;
    uid_t euid;
    uid_t suid;
    uid_t fsuid;

    gid_t rgid;
    gid_t egid;
    gid_t sgid;
    gid_t fsgid;

    mode_t umask;
} Config;

typedef struct {
    char *path;
    mode_t mode;
} ModifiedNode;
