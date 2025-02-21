#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <pwd.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>

int isdigitstr(const char *s)
{
    if (*s == '\0')
  return -1;

    char *endp;
    long long rc;

    strtoll(s, &endp, 10);

    return rc = (*s != '\0' && *endp == '\0') ?
        0 : -1;
}

char *dtoa(long long n)
{
    static char buf[100], *bufp;
    char *l, *r, tmp;
    int sign = (n < 0) ? 1 : 0;

    bufp = buf;
    if (n == 0) {
  *(bufp++) = '0';
    }
    else
    {
        if (sign)
        {
      n = -n;
      *(bufp++) = '-';
        }

        while (n)
        {
      *(bufp++) = (n % 10) + '0';
      n /= 10;
        }
    }

    l = sign ? (buf + 1) : buf;
    r = bufp - 1;

    while (l < r)
    {
  tmp = *l;
  *l++ = *r;
  *r-- = tmp;
    }

    *bufp = '\0';

    return buf;
}

int
main(int argc, char *argv[])
{
    if (argc != 2) 
    {
  fprintf(stderr, "usage: %s <path>\n", argv[0]);
  exit(EXIT_FAILURE);
    }

    DIR *dirp_out;
    struct dirent *dirent;
    char path[PATH_MAX], symbuf[PATH_MAX];
    char *comm_pathendp;
    char *s;
    int fd, fdnum;
    int rc;
    int perrno;

    if ((dirp_out = opendir("/proc")) == NULL) 
    {
  perror("opendir");
  exit(EXIT_FAILURE);
    }
    
    perrno = errno;
    while ((dirent = readdir(dirp_out)) != NULL)
    {
        if (isdigitstr(dirent->d_name) == -1)
            continue;

        if (snprintf(path, sizeof(path), "/proc/%s/fd/", dirent->d_name) < 0)
        {
      perror("sprintf");
      continue;
        }
        
        for (fdnum = 0; fdnum < FOPEN_MAX; ++fdnum) 
        {
      s = dtoa(fdnum);
      strcat(path, s);
      errno = 0;
        if ((fd = readlink(path, symbuf, sizeof(symbuf) - 1)) != -1)
        {
      symbuf[fd] = '\0';
        if (strcmp(argv[1], symbuf) == 0)
        {
      printf("PID: %s\n", dirent->d_name);
      break;
        }
        }

        path[strlen(path) - strlen(s)] = '\0';
        }

        errno = perrno;
    }
    if (perrno != errno)
  perror("readdir");
    
    closedir(dirp_out);
    exit(EXIT_SUCCESS);
}
