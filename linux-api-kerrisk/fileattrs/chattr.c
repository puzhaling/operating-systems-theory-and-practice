#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/fs.h>

typedef enum __chattr_act_t {
    CHACT_ADD, 
    CHACT_SUB, 
    CHACT_ASN
} chattr_act_t;

void chattr(const char *path, const char *attrs, chattr_act_t chattr_act) {
    int fd;
    int currattrs, newattrs = 0;
    size_t nattrs = strlen(attrs);

    if ((fd = open(path, O_RDONLY)) == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < nattrs; ++i) {
        switch (attrs[i]) {
            case 'a': newattrs |= FS_APPEND_FL;  break;
            case 'A': newattrs |= FS_NOATIME_FL; break;
            case 'c': newattrs |= FS_COMPR_FL;   break;
            case 'd': newattrs |= FS_NODUMP_FL;  break;
            case 'D': newattrs |= FS_DIRSYNC_FL; break;
            case 'e': newattrs |= FS_EXTENT_FL;  break;
            // Add more attributes as needed
            default:
                fprintf(stderr, "unknown attr: %c\n", attrs[i]);
                close(fd);
                exit(EXIT_FAILURE);
        }
    }

    if (ioctl(fd, FS_IOC_GETFLAGS, &currattrs) == -1) {
        perror("ioctl");
        close(fd);
        exit(EXIT_FAILURE);
    }

    if (chattr_act == CHACT_ADD) {
        currattrs |= newattrs;
    } else if (chattr_act == CHACT_SUB) {
        currattrs &= ~newattrs;
    } else if (chattr_act == CHACT_ASN) {
        currattrs = newattrs;
    } else {
        fprintf(stderr, "unknown chattr_act action\n");
        close(fd);
        exit(EXIT_FAILURE);
    }

    if (ioctl(fd, FS_IOC_SETFLAGS, &currattrs) == -1) {
        perror("ioctl");
        close(fd);
        exit(EXIT_FAILURE);
    }

    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s mode files...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    chattr_act_t chattr_act;
    switch (argv[1][0]) {
        case '+': chattr_act = CHACT_ADD; break;
        case '-': chattr_act = CHACT_SUB; break;
        case '=': chattr_act = CHACT_ASN; break;
        default:
            fprintf(stderr, "mode arg has invalid format\n");
            exit(EXIT_FAILURE);
    }

    char attrs[20];
    strncpy(attrs, argv[1] + 1, sizeof(attrs) - 1);
    attrs[sizeof(attrs) - 1] = '\0';

    for (int i = 2; i < argc; ++i) {
        chattr(argv[i], attrs, chattr_act);
    }

    exit(EXIT_SUCCESS);
}
