#include <sys/resource.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include "../essentials.h"

void sigHandler(int sig) {
        if (sig == SIGXFSZ)
                fprintf(stderr, "SIGXFSZ signal was caught\n");
        else
                fprintf(stderr, "??? signal was caught\n");
}

int main(void) {
        int              fd;
        struct rlimit    rlimit;
        struct sigaction sa;

        sa.sa_handler = sigHandler;
        sa.sa_flags = 0;
        sigemptyset(&sa.sa_mask);

        if (sigaction(SIGXFSZ, &sa, NULL) == -1)
                errExit("sigaction");
        
        fd = open("file.txt", O_WRONLY | O_CREAT, S_IRWXU);
        if (fd == -1)
                errExit("open");

        if (getrlimit(RLIMIT_FSIZE, &rlimit) == -1)
                errExit("getrlimit");
        
        printf("RLIMIT_FSIZE (before):\n");
        printf("soft limit = %lld\n", (long long) rlimit.rlim_cur);
        printf("hard limit = %lld\n", (long long) rlimit.rlim_max);
        
        rlimit.rlim_cur = 10;
        if (setrlimit(RLIMIT_FSIZE, &rlimit) == -1)
                errExit("setrlimit: RLIMIT_FSIZE");
        
        if (getrlimit(RLIMIT_FSIZE, &rlimit) == -1)
                errExit("getrlimit");
        
        printf("RLIMIT_FSIZE (after):\n");
        printf("soft limit = %lld\n", (long long) rlimit.rlim_cur);
        printf("hard limit = %lld\n", (long long) rlimit.rlim_max);

        for (;;) {
                if (write(fd, "a", 1) == -1) {
                        if (errno == EFBIG)
                                fprintf(stderr, "write: EFBIG\n");
                        else
                                perror("write");
                break;
                }
        }
                
        close(fd);
        exit(EXIT_SUCCESS);
}
