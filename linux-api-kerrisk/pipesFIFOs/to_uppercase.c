#include <unistd.h>
#include <stdio.h>
#include "../essentials.h"

static
void upper(char *buf, int siz) {
        int i;

        for (i = 0; i < siz; ++i) {
                if (97 <= buf[i] && buf[i] <= 122)
                        buf[i] -= 32;
        }
}

int 
main(int argc, char **argv) {
        int sendPipe[2], recievePipe[2];
        char buf[1025];
        ssize_t nread, nwrite;
        int responseLen;

        if (pipe(sendPipe) == -1)
                errExit("pipe");
        if (pipe(recievePipe) == -1)
                errExit("pipe");
        
        switch (fork()) {
        case -1:
                errExit("fork");
        case 0:
                if (close(sendPipe[1]) == -1)
                        errExit("children: close 1");
                if (close(recievePipe[0]) == -1)
                        errExit("children: close 0");
                
                for (;;) {
                        nread = read(sendPipe[0], buf, 1024);
                        if (nread < 0)
                                errExit("child: read");
                        if (nread == 0)
                                break;
#ifdef DEBUG
                        printf("child: read: %d bytes from sendPipe\n", 
                                        (int) nread);
#endif

                        upper(buf, nread);

                        nwrite = write(recievePipe[1], buf, nread);
                        if (nwrite == -1)
                                errExit("parent: write");
#ifdef DEBUG
                        printf("child: write: %d bytes to recievePipe\n", 
                                        (int) nwrite);
#endif
                }

                if (close(sendPipe[0]) == -1)
                        warn("parent: close 1");
                if (close(recievePipe[1]) == -1)
                        warn("parent: close 0");

                _exit(EXIT_SUCCESS);
        default: 
                break;
        }

        if (close(sendPipe[0]) == -1)
                errExit("parent: close 0");
        if (close(recievePipe[1]) == -1)
                errExit("parent: close 1");
        
        for (;;) {
                nread = read(STDIN_FILENO, buf, 1024);
                if (nread < 0)
                        errExit("parent: read");
#ifdef DEBUG
                printf("parent: read: %d bytes from STDIN\n", 
                                (int) nread);
#endif

                nwrite = write(sendPipe[1], buf, nread);
                if (nwrite == -1)
                        errExit("parent: write");
                else if (nwrite < nread)
                        warn("parent: partial write");
#ifdef DEBUG
                printf("parent: write: %d bytes to sendPipe\n", 
                                (int) nwrite);
#endif

                responseLen = read(recievePipe[0], buf, 1024);
                if (responseLen == -1)
                        errExit("parent: read");
                else if (responseLen < nread)
                        warn("parent: data is lost");
#ifdef DEBUG
                printf("parent: read: %d bytes from recievePipe\n", 
                                (int) nread);
#endif
                buf[responseLen] = '\0';

                printf("result: %s", buf);
        }

        if (close(sendPipe[1]) == -1)
                warn("parent: close 1");
        if (close(recievePipe[0]) == -1)
                warn("parent: close 0");

        exit(EXIT_SUCCESS);
}
