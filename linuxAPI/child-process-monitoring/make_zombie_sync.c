#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <errno.h>
#include "../essentials.h"

#define CMD_SIZE 200

void sigchldHandler(int)
{
}

int 
main(int argc, char *argv[])
{
    char cmd[CMD_SIZE];
    pid_t childPid;
    struct sigaction sa;
    sigset_t blockMask, emptyMask;

    setbuf(stdout, NULL);

    printf("Parent PID=%ld\n", (long) getpid());
    
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sigchldHandler;
    
    sigemptyset(&emptyMask);
    sigemptyset(&blockMask);
    sigaddset(&blockMask, SIGCHLD);
    if (sigprocmask(SIG_SETMASK, &blockMask, NULL) == -1)
        errExit("sigprocmask - SIG_SETMASK");
    
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
        errExit("sigaction");
    
    switch (childPid = fork()) {
    case -1:
        errExit("fork");

    case 0:
        printf("Child (PID=%ld) exiting\n", (long) getpid());
        _exit(EXIT_SUCCESS);

    default:
        printf("Waiting for SIGCHLD...\n");
        if (sigsuspend(&emptyMask) == -1 && errno != EINTR)
            errExit("sigsuspend - invalid mask");

        snprintf(cmd, CMD_SIZE, "ps | grep %s", basename(argv[0]));

        if (kill(childPid, SIGKILL) == -1)
            errExit("kill");

        if (waitpid(childPid, NULL, 0) == -1)
            errExit("waitpid");

        printf("After sending SIGKILL to zombie (PID=%ld):\n",
               (long) childPid);
        system(cmd);
        
        if (sigprocmask(SIG_SETMASK, &emptyMask, NULL) == -1)
            errExit("sigprocmask - SIG_SETMASK");

        exit(EXIT_SUCCESS);
    }
}
