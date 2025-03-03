#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>

int main(void)
{
        sigset_t pendingMask;
        struct sigaction sigact;
        int rc;
        
        sigact.sa_handler = SIG_IGN;
        if (sigaction(SIGUSR1, &sigact, NULL) == -1) {
                perror("sigaction");
                exit(EXIT_FAILURE);
        }
        
        if (kill(getpid(), SIGUSR1) == -1) {
                perror("kill");
                exit(EXIT_FAILURE);
        }

        sigpending(&pendingMask);

        rc = sigismember(&pendingMask, SIGUSR1);
        if (rc == 1)
                printf("wtf?!\n");
        else if (rc == 0) {
                printf("not a member\n");
        } else {
                perror("sigpending");
                exit(EXIT_FAILURE);
        }
        
        exit(EXIT_SUCCESS);
}
