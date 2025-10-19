#include <sys/types.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include "../essentials.h"

void sigHandler(int) 
{
}

int main(void) {
        pid_t            pid;
        struct sigaction sa;
        sigset_t         emptyMask, blockMask;
        struct rusage    rusage;
        volatile int     i;
        
        sigemptyset(&blockMask);
        sigemptyset(&emptyMask);
        sigemptyset(&sa.sa_mask);
        sigaddset(&sa.sa_mask, SIGUSR1);
        sa.sa_flags = 0;
        sa.sa_handler = sigHandler;
        if (sigaction(SIGUSR1, &sa, NULL) == -1)
                errExit("Parent: sigaction"); 
        
        sigaddset(&blockMask, SIGUSR1);
        if (sigprocmask(SIG_SETMASK, &blockMask, NULL) == -1)
                errExit("Parent: sigprocmask");

        pid = fork();
        if (pid == -1)
                errExit("fork");
        else if (pid == 0) {
                if (kill(getppid(), SIGUSR1) == -1)
                        err_exit("Child: kill");
                
                for (i = 0; i < 1e6; ++i)
                        ;

                _exit(EXIT_SUCCESS);
        }

        sigsuspend(&emptyMask);

        if (getrusage(RUSAGE_CHILDREN, &rusage) == -1)
                errExit("Parent: getrusage before wait");
        printf("Parent: children spent microsecs (before wait) %ld\n",
                (long) rusage.ru_utime.tv_usec);

        wait(NULL);

        if (getrusage(RUSAGE_CHILDREN, &rusage) == -1)
                errExit("Parent: getrusage after wait");
        printf("Parent: children spent microsecs (after wait) %ld\n",
                (long) rusage.ru_utime.tv_usec);

        exit(EXIT_SUCCESS);
}
