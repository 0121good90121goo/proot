/** Determines whether the file specified by path exists.
 */
int path_exists(char path[PATH_MAX])
{
    return access(path, F_OK);  
}
