#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

void arX(const char *path)
{
    struct stat st;
    mode_t perms;

    if (stat(path, &st) == -1)
        {
        perror("stat");
        exit(EXIT_FAILURE);
        }
   
    perms = S_IRUSR | S_IRGRP | S_IROTH;
    if ((st.st_mode & S_IFDIR) ||
        (st.st_mode & S_IXUSR) ||
        (st.st_mode & S_IXGRP) ||
        (st.st_mode & S_IXOTH))
        {
        perms |= (S_IXUSR | S_IXGRP | S_IXOTH);    
        }

    if (chmod(path, perms) == -1)
        {
        perror("chmod");
        exit(EXIT_FAILURE);
        }
}

int main(int argc, char *argv[])
{
    if (argc == 1)
        {
        fprintf(stderr, "usage: %s {FILE,DIR}...\n", argv[0]);
        exit(EXIT_FAILURE);
        }

    int i;
    for (i = 0; i < argc; ++i)
        {
        arX(argv[i]); 
        }

    exit(EXIT_SUCCESS);
}
