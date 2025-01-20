#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/types.h>

#define BUFF_SIZE 511

int
main(int argc, char *argv[]) 
{
    int i, j;
    int n_read, n_write;
    char buff[BUFF_SIZE + 1];
    int fd, fds[argc - 1];
    int apnd;
    int open_flags;
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
                  S_IROTH | S_IWOTH;

    if (
        argc == 1 ||
        argc == 2 && (strcmp(argv[1], "-a") == 0)
       )
    {
        fputs("usage: mytee [-a] FILE...\n", stderr);
        exit(EXIT_FAILURE);
    }


    apnd = 0;
    if (strcmp(argv[1], "-a") == 0)
        apnd = 1;
    
    if (apnd)
        open_flags = O_APPEND | O_WRONLY | O_CREAT;
    else
        open_flags = O_WRONLY | O_TRUNC | O_CREAT;
    
    j = 0;
    for (i = 1; i < argc; ++i) {
        fd = open(argv[i], open_flags, mode);

        if (fd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }

        if (fd != -1)
            fds[j++] = fd;
    }
    fds[j] = -1;

    while ((n_read = read(STDIN_FILENO, buff, BUFF_SIZE)) > 0) {
        if (n_read == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        n_write = write(STDOUT_FILENO, buff, n_read);   

        for (j = 0; j < argc - 1; j++) {
            if (fds[j] == -1)
                break;

            n_write = write(fds[j], buff, n_read);   

            if (n_write == -1) {
                perror("write");
                exit(EXIT_FAILURE);
            }
        }
    }

    for (j = 0; j < argc - 1; ++j) {
        if (fds[j] == -1)
            break;

        close(fds[j]);
    } 

    return 0;
}
