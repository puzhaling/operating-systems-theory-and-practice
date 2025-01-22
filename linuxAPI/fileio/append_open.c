#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

int main(void)
{
  int fd;
  off_t offset;
  size_t nwrite;
  char buff[] = "test";

  fd = open("test.txt", O_WRONLY | O_APPEND);
  if (fd == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  offset = lseek(fd, 0, SEEK_SET);
  if (offset == -1) {
    perror("lseek");
    exit(EXIT_FAILURE);
  }

  nwrite = write(fd, buff, sizeof(buff));
  if (nwrite < 0) {
    perror("write");
    exit(EXIT_FAILURE);
  }

  if (close(fd) == -1) {
    perror("write");
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}
