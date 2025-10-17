#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>

static void
fpathconfPrint(const char *msg, int fd, int name)
{
    long lim;

    errno = 0;
    lim = fpathconf(fd, name);
    if (lim != -1) {
        printf("%s %ld\n", msg, lim);
    } else {
        if (errno == 0) {
            printf("%s (indeterminate)\n", msg);
        } else {
            perror("fpathconf");
            exit(EXIT_FAILURE);
        }
    }
}

int main(void)
{
    fpathconfPrint("_PC_NAME_MAX:", STDIN_FILENO, _PC_NAME_MAX);
    fpathconfPrint("_PC_PATH_MAX:", STDIN_FILENO, _PC_PATH_MAX);
    fpathconfPrint("_PC_PIPE_BUF:", STDIN_FILENO, _PC_PIPE_BUF);

    exit(EXIT_SUCCESS);
}
