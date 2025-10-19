#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int dup(int oldfd)
{
  int newfd;
  newfd = fcntl(oldfd, F_DUPDF, 0);
  return newfd;
}

bool fd_is_valid(int fd)
{
  return fcntl(fd, F_GETFD) != -1 || errno != EBADF;
}

int dup2(int oldfd, int newfd)
{
  if (fd_is_valid(oldfd)) {

    if (oldfd == newfd) {
      return newfd;
    }

    close(newfd);
    return dup(oldfd);
  } 
  else {
    return -1;
  }
}

