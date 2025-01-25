#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include <malloc.h>

#include <sys/types.h>
#include <sys/uio.h>

/*
ssize_t readv(int fd, const struct iovec *iov, int iovcnt)
{
  int i;
  size_t totalread = 0;
  size_t justread;
  for (i = 0; i < iovcnt; ++i) {
    justread = read(fd, iov[i].iov_base, iov[i].iov_len);
    if (justread < 0)
      return -1;

    totalread += justread;
  }
  return totalread;
}

ssize_t writev(int fd, const struct iovec *iov, int iovcnt)
{
  return 1;
}
*/



typedef struct __X {
  int n;
} X;

int
main(int argc, char *argv[])
{
  int fd = open("test.txt", O_RDWR | O_APPEND);
  struct iovec iov[2];

  X *x = malloc(sizeof(X));
  char *_x = malloc(1);
  
  iov[0].iov_base = x;
  iov[0].iov_len = sizeof(x);
  iov[1].iov_base = _x;
  iov[1].iov_len = sizeof(_x);

  readv(fd, iov, 2);
  writev(fd, iov, 2);



  close(fd);
}
