#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <sys/types.h>
#include <grp.h>

int initgroups(const char *user, gid_t group)
{
    int rc, i, ngid;
    uid_t euid;
    gid_t gidlist[NGROUPS_MAX + 1];
    struct group *grp;

    ngid = 0;
    while ( (grp = getgrent()) != NULL) {
        for (i = 0; grp->gr_mem[i] != NULL; i++) {
            if (strcmp(grp->gr_mem[i], user) == 0) {
                if (ngid >= NGROUPS_MAX) {
                    endgrent();
                    errno = EINVAL;
                    return -1;
                }
                gidlist[ngid++] = grp->gr_gid;
                break;
            }
        }
    }
    endgrent();
    
    if (ngid >= NGROUPS_MAX) {
        errno = EINVAL;
        return -1;
    }

    gidlist[ngid++] = group;

    euid = geteuid();
    if (seteuid(0) == -1) {
        return -1;
    }

    rc = setgroups(ngid, gidlist);

    if (seteuid(euid) == -1) {
        return -1;
    }

    return rc;

    euid = geteuid();
    seteuid(0);
    setgroups(ngid, gidlist); 
    seteuid(euid);

    return 0;
}
