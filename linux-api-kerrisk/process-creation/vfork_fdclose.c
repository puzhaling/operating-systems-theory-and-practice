#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "../essentials.h"

int main(void)
{
    int fd = open("testfile.txt", O_RDWR | O_CREAT, 0644);
    if (fd == -1)
        errExit("open");

    pid_t pid = vfork();
    if (pid < 0) 
        errExit("vfork");
    else if (pid == 0) {
        printf("child process: closing stdin\n");
        close(0);

        char buf[10];
        ssize_t n = read(0, buf, sizeof(buf));
        if (n == -1)
            perror("read in child");
        else
            printf("child process: read %zd bytes\n", n);
       
        _exit(0);
    } else {
        printf("parent process: waiting for child process end\n");

        char buf[10];
        ssize_t n = read(0, buf, sizeof(buf));
        if (n == -1)
            perror("read in parent");
        else 
            printf("parent process: read %zd bytes\n", n);
       
    }

    close(fd);
    exit(EXIT_SUCCESS);
}
