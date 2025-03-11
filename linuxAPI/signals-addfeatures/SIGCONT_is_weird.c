#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#include "../essentials.h"

void sigcont_handler(int sig)
{
    printf("SIGCONT handler is invoked\n"); // UNSAFE
}

int main(void)
{
    sigset_t blockMask, prevMask;
    struct sigaction sa;
    
    sa.sa_handler = sigcont_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    sigemptyset(&blockMask);
    sigemptyset(&prevMask);
    sigaddset(&blockMask, SIGCONT);

    printf("PID: %ld\n", (long) getpid()); 

    // block SIG_CONT
    if (sigprocmask(SIG_BLOCK, &blockMask, &prevMask) == -1)
        errExit("sigprocmask - SIG_BLOCK");

    printf("SIGCONT is blocked\n");
    
    // setting handler for SIG_CONT
    if (sigaction(SIGCONT, &sa, NULL) == -1)
        errExit("sigaction - SIGCONT");

    printf("Raising SIGSTOP...\n");
    sleep(2);

    // stopping process
    raise(SIGSTOP);
    
    // waiting for SIGCONT

    printf("SIGCONT delivered, but where is the handler?\n");
    sleep(3);

    // unblocking SIGCONT

    if (sigprocmask(SIG_SETMASK, &prevMask, NULL) == -1)
        errExit("sigprocmask - SIG_SETMASK");
    
    exit(EXIT_SUCCESS);
}
