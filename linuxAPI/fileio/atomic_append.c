#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int
main(int argc, char *argv[])
{
  int flags = O_WRONLY | O_APPEND | O_CREAT;
  const mode_t mode = S_IRUSR | S_IWUSR;
  off_t offset;
  int fd;
  int old_errno;
  size_t nwrite;
  unsigned long long n_bytes_to_add;
  char byte[] = "";
  
  if (argc != 3 && argc != 4) {
    perror("usage: atomic_append filename num-bytes [x]\n");
    exit(EXIT_FAILURE);
  }

  if (argc == 4) {
    flags ^= O_APPEND; 
  }

  fd = open(argv[1], flags, mode);
  if (fd == -1) {
    perror("passed filename");
    exit(EXIT_FAILURE);
  }

  old_errno = errno;
  n_bytes_to_add = atoll(argv[2]);
  if (old_errno != errno && n_bytes_to_add == 0) {
    perror("passed num-bytes");
    exit(EXIT_FAILURE);
  }
  
  
  while (n_bytes_to_add--) {
    offset = lseek(fd, 0, SEEK_END);
    if (offset == -1) {
      perror("lseek");
      exit(EXIT_FAILURE);
    }

    nwrite = write(fd, byte, sizeof(byte));
    if (nwrite == -1) {
      perror("write");
      exit(EXIT_FAILURE);
    }
  }

  if (close(fd) == -1) {
    perror("close");
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}
