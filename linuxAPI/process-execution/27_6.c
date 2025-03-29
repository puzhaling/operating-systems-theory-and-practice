#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "../essentials.h"

void chldHandler(int)
{
    printf("SIGCHLD handler is running..\n");
}

int main(void)
{
    pid_t childPid;
    struct sigaction chldSaNew, chldSaOld;
    sigset_t blockMask, prevMask;
    
    /* setting handler for SIGCHLD */
    chldSaNew.sa_flags = 0;
    chldSaNew.sa_handler = chldHandler;
    sigemptyset(&chldSaNew.sa_mask);

    sigaction(SIGCHLD, &chldSaNew, &chldSaOld);
    
    /* blocking SIGCHLD */
    sigemptyset(&blockMask);
    sigemptyset(&prevMask);
    sigaddset(&blockMask, SIGCHLD);
    sigprocmask(SIG_BLOCK, &blockMask, &prevMask);
    
    if (sigismember(&blockMask, SIGCHLD) == 1)
        printf("SIGCHLD is blocked.\n");
    else
        errExit("Something is wrong with blocking SIGCHLD.\n");
        
        
    childPid = fork();
    if (childPid == 0) {
        /* some child work */ 
        system("ls | wc");
    } else {
        printf("Parent: start to wait\n");
        wait(NULL);
        printf("Parent: wait is end.\n");
        printf("Parent: unblocking SIGCHLD...\n");
        sigprocmask(SIG_UNBLOCK, &blockMask, NULL);
    }

    exit(EXIT_SUCCESS);
}
