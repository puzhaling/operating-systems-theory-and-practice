/*
 * protocols all creates, deletions and renames
 * inside dirdirpath, that was passed as command-line
 * argument
 */

#define _XOPEN_SOURCE 500

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <ftw.h>
#include <stdint.h>
#include <sys/inotify.h>
#include <sys/stat.h>

#define BUFLEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

static int gl_inotifyfd = -1;
static char gl_eventbuf[BUFLEN] = "";
static uint32_t gl_mask = 0;

void display_inotify_event(struct inotify_event *event)
{
        printf("event for: %s\n", event->name);
        if (event->mask & IN_CREATE)            printf("IN_CREATE ");
        if (event->mask & IN_DELETE)            printf("IN_DELETE ");
        if (event->mask & IN_DELETE_SELF)       printf("IN_CREATE ");
        if (event->mask & IN_MOVED_FROM)        printf("IN_MOVED_FROM ");
        if (event->mask & IN_MOVED_TO)          printf("IN_MOVED_TO ");
        if (event->mask & IN_MOVED_FROM)        printf("IN_MOVED_SELF ");
        printf("\n");
}

int 
process(const char *fpath, const struct stat *sb,
        int typeflag, struct FTW *ftwbuf)
{       
        int wd;

        if (S_ISDIR(sb->st_mode)) {
                if ((wd = inotify_add_watch(gl_inotifyfd, fpath, gl_mask)) 
                    == -1) {
                        fprintf(stderr, "%s: ", fpath);
                        perror("inotify_add_watch");
                }
        }
        return 0;
}
        
int
main(int argc, char **argv)
{
        if (argc < 2 || strcmp(argv[1], "--help") == 0) {
                fprintf(stderr, "usage: %s dirpath\n", *argv);
                exit(EXIT_FAILURE);
        }

        int wd;
        ssize_t nread;
        char dirpath[PATH_MAX];
        char *p;
        struct inotify_event *event;
        uint32_t mask;
        int rc;

        if ( (gl_inotifyfd = inotify_init()) == -1) {
                perror("inotify_init");
                exit(EXIT_FAILURE);
        }
                
        snprintf(dirpath, PATH_MAX, "%s", argv[1]);
        
        gl_mask = IN_CREATE | IN_DELETE | IN_DELETE_SELF |
                  IN_MOVE;

        if ( (wd = inotify_add_watch(gl_inotifyfd, dirpath, gl_mask)) == -1) {
                perror("inotify_add_watch");
                exit(EXIT_FAILURE);
        }
        
        if ( (rc = nftw(dirpath, process, 10, 0)) == -1)
                perror("nftw: opening subdirs");

        for (;;) {
                nread = read(gl_inotifyfd, gl_eventbuf, BUFLEN); 
                if (nread == 0) {
                        fprintf(stderr, "read(2) from "
                                "inotifyfd returned 0!");
                        abort();
                }
                
                if (nread == -1) {
                        perror("read");
                        exit(EXIT_FAILURE);
                }

                for (p = gl_eventbuf; p < gl_eventbuf + nread; ) {
                        event = (struct inotify_event *) p;
                        display_inotify_event(event); 
                        p += sizeof(struct inotify_event) + event->len;
                }
        }
        
        close(gl_inotifyfd);
        exit(EXIT_SUCCESS);
}
