#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>

#define BUFSIZ 1024

int 
main(int argc, char *argv[])
{
    if (argc != 2 && argc != 4) 
        {
        fprintf(stderr, "usage: %s [ -n num ] file\n", argv[0]);
        exit(EXIT_FAILURE);
        }
    
    int nlines, fd;
    char *endptr;
    char buf[BUFSIZ];
    const char *path;
    off_t filesiz, pos;
    size_t bytesToRead;
    ssize_t bytesRead, i;

    if (argc == 2) 
        {
        nlines = 10;
        path = argv[1];
        } 
    else 
        { 
        errno = 0;
        nlines = (int) strtol(argv[2], &endptr, 10);
        if (errno != 0) 
            {
            perror("strtol");
            exit(EXIT_FAILURE);
            }
        if (endptr == argv[2]) 
            {
            fprintf(stderr, "no digits were found\n");
            exit(EXIT_FAILURE);
            }
        if (nlines < 0) 
            {
            fprintf(stderr, "negative number or lines\n");
            exit(EXIT_FAILURE);
            }

        path = argv[3];
    }

    if ((fd = open(path, O_RDONLY)) == -1) 
        {
        perror("open");
        exit(EXIT_FAILURE);
        }

    filesiz = lseek(fd, 0, SEEK_END);
    if (filesiz == -1) 
        {
        perror("lseek");
        exit(EXIT_FAILURE);
        }
    
    pos = filesiz;
    while (pos > 0 && nlines > 0) 
        {
        bytesToRead = (pos > BUFSIZ) ? BUFSIZ : pos;
        pos -= bytesToRead;
        lseek(fd, pos, SEEK_SET);
        bytesRead = read(fd, buf, bytesToRead);
        if (bytesRead == -1) 
            {
            perror("read");
            exit(EXIT_FAILURE);
            }
        for (i = bytesRead - 1; i >= 0; --i) 
            {
            if (buf[i] == '\n') 
                {
                nlines--;
                if (nlines < 0) 
                    {
                    pos += (i + 1);
                    break;
                    }
                }
            }
        }
    
    if (lseek(fd, pos, SEEK_SET) == -1) 
        {
        perror("lseek");
        exit(EXIT_FAILURE);
        }

    while ((bytesRead = read(fd, buf, BUFSIZ)) > 0) 
        {
        if (write(STDOUT_FILENO, buf, bytesRead) != bytesRead) 
            {
            perror("write");
            exit(EXIT_FAILURE);
            }
        }

    if (bytesRead == -1) 
        {
        perror("read");
        exit(EXIT_FAILURE);
        }

    close(fd);
    return 0;
}
