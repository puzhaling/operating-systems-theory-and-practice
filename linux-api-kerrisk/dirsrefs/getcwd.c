#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

char *getcwd(char *buf, size_t size)
{
        char path[PATH_MAX], tmp[PATH_MAX];
        struct stat cst, pst, st;
        DIR *dp;
        struct dirent *dirent;
        dev_t root_dev;
        ino_t root_ino;

        if (stat("/", &st) == -1) {
                perror("stat on root dir");
                exit(EXIT_FAILURE);
        }
        root_dev = st.st_dev;
        root_ino = st.st_ino;

        if (stat(".", &cst) == -1) {
                perror("stat on cwd");
                exit(EXIT_FAILURE);
        }

        path[0] = '\0';

        while (1) {
                if (stat("..", &pst) == -1) {
                        perror("stat on parent dir");
                        exit(EXIT_FAILURE);
                }

                if (cst.st_dev == root_dev && cst.st_ino == root_ino)
                        break;

                if ( (dp = opendir("..")) == NULL) {
                        perror("opendir on parent");
                        exit(EXIT_FAILURE);
                }

                chdir("..");

                while ( (dirent = readdir(dp)) != NULL) {
                        if (strcmp(dirent->d_name, ".") == 0 ||
                            strcmp(dirent->d_name, "..") == 0)
                                continue;
                        
                        if (stat(dirent->d_name, &st) == -1) {
                                perror("stat");
                                exit(EXIT_FAILURE);
                        }

                        if (st.st_ino == cst.st_ino && 
                            st.st_dev == cst.st_dev) {
                                snprintf(tmp, PATH_MAX, "/%s%s", 
                                         dirent->d_name, path);
                                strncpy(path, tmp, PATH_MAX);
                                break;
                        }
                }

                closedir(dp);
                cst = pst;
        }

        if (strlen(path) == 0) {
                strncpy(path, "/", PATH_MAX);
        }

        strncpy(buf, path, size);
        chdir(path);

        return buf;
}

int main(void)
{
        char buf[PATH_MAX];
        printf("%s\n", getcwd(buf, PATH_MAX) );
}
