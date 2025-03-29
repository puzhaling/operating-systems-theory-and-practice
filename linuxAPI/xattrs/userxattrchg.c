#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/xattr.h>

static
void usagerr(const char *prognam)
{
    fprintf(stderr, "usage: %s -n name [ -v value ] pathname\n", prognam);
    exit(EXIT_FAILURE);
}

// a.out -n name [-v value] pathname...
int
main(int argc, char *argv[])
{
    if (argc < 3)
    {
        usagerr(*argv);
    }

    int opt;
    int i;
    const char *nam = NULL, *val = NULL;
    int hasnam = 0, hasval = 0;

    while ((opt = getopt(argc, argv, "n:v:")) != -1)
    {
        switch (opt)
        {
        case 'n':
            nam = optarg;
            hasnam = 1;
            break;
        case 'v':
            val = optarg;
            hasval = 1;
            break;
        default:
            usagerr(*argv);
        }
    }

    if (!hasnam)
    {
        usagerr(*argv);
    }

    if (optind >= argc)
    {
        usagerr(*argv);
    }

    for (i = optind; i < argc; ++i)
    {
        const char *pathname = argv[i];

        if (hasval)
        {
            if (setxattr(pathname, nam, val, strlen(val), 0) == -1)
            {
                perror(pathname); 
            }
        }
        else
        {
            // Если значение не указано, удаляем атрибут
            if (removexattr(pathname, nam) == -1)
            {
                perror(pathname);
            }
        }
    }

    exit(EXIT_SUCCESS);
}
