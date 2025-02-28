#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ftw.h>

int regulars = 0;
int dirs = 0;
int chardevs = 0;
int blokdevs = 0;
int fifodevs = 0;
int sockets = 0;
int symlinks = 0;

void echostatsmsgs()
{
        const size_t all = regulars + dirs + chardevs + 
                blokdevs + fifodevs + sockets + symlinks;
        
        if (all == 0) {
                printf("no files\n");
                return;
        }
  
        printf("regulars: %d (%.2f%)\n", regulars, (float)regulars * 100 / all);
        printf("dirs:     %d (%.2f%)\n", dirs, (float)dirs * 100 / all);
        printf("chardevs: %d (%.2f%)\n", chardevs, (float)chardevs * 100 / all);
        printf("blokdevs: %d (%.2f%)\n", blokdevs, (float)blokdevs * 100 / all);
        printf("fifodevs: %d (%.2f%)\n", fifodevs, (float)fifodevs * 100 / all);
        printf("sockets:  %d (%.2f%)\n", sockets, (float)sockets * 100 / all);
        printf("symlinks: %d (%.2f%)\n", symlinks, (float)symlinks * 100 / all);
}

int incstats(const char *fpath, const struct stat *sb,
             int typeflag, struct FTW *ftwbuf)
{
  mode_t mode;
  
  mode = sb->st_mode;
  if (mode & S_IFREG)
    ++regulars;
  else if (mode & S_IFDIR)
    ++dirs;
  else if (mode & S_IFCHR)
    ++chardevs;
  else if (mode & S_IFBLK)
    ++blokdevs;
  else if (mode & S_IFIFO)
    ++fifodevs;
  else if (mode & S_IFSOCK)
    ++sockets;
  else if (mode & S_IFLNK)
    ++symlinks;
  return 0;
}

int 
main(int argc, char **argv)
{
  if (argc != 2)
    {
      fprintf(stderr, "usage: %s <starting branch path>\n", *argv);
      exit(EXIT_FAILURE);
    }

  if (nftw(argv[1], incstats, 10, 0) == -1)
    {
      perror("nftw");
      exit(EXIT_FAILURE);
    }
  
  echostatsmsgs();
  exit(EXIT_SUCCESS);
}
