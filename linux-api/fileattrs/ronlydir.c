#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(void)
{
    struct stat st;
    int fd;
    mode_t mode;
    char dirp[PATH_MAX];

    mode = S_IRUSR | S_IWUSR; /* rw- */
    sprintf(dirp, "%s/test", getenv("PWD"));
    if ((fd = mkdir(dirp, mode)) == -1)
        {
        perror("mkdir");
        exit(EXIT_FAILURE);
        }


}
