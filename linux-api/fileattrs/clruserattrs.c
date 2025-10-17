#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(void)
{
    int fd;
    struct stat sb;
    mode_t mode;
    
    mode = S_IRUSR | S_IWUSR | S_IXUSR |
           S_IROTH | S_IWOTH | S_IXOTH;

    if ((fd = creat("test", mode)) == -1) 
        {
        perror("creat");
        exit(EXIT_FAILURE);
        }

    if (fstat(fd, &sb) == -1)
        {
        perror("fstat");
        exit(EXIT_FAILURE);
        }
    
    mode ^= mode;
    mode = sb.st_mode & ~S_IRUSR & ~S_IWUSR & ~S_IXUSR;

    if (fchmod(fd, mode) == -1)
        {
        perror("fchmod");
        exit(EXIT_FAILURE);
        }

    close(fd);
    exit(EXIT_SUCCESS);
}
