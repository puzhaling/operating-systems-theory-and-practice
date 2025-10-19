#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(void)
{
    mode_t umask_val;
    umask(umask_val = umask(0));

    printf("umask value for current proc is %o\n", umask_val);

    exit(EXIT_SUCCESS);
}
