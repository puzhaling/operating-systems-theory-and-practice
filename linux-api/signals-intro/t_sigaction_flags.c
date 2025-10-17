
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include "../essentials.h"

void resethand_handler(int sig)
{
        printf("you will never see me again\n");
}

void nodefer_handler(int sig)
{
        printf("A\n");
        sleep(2);
        printf("B\n");
}

int main(void)
{
        struct sigaction sigact;
        int i;
        
        // ? uncomment to test SA_RESETHAND flag ?
        //
        //sigact.sa_handler = resethand_handler;
        //sigact.sa_flags = SA_RESETHAND;
        //sigemptyset(&sigact.sa_mask);

        //if (sigaction(SIGUSR1, &sigact, NULL) == -1)
        //        errExit("sigact");

        //for (i = 0; i < 2; ++i)
        //        pause();

        sigact.sa_handler = nodefer_handler;
        sigact.sa_flags = SA_NODEFER;
        sigemptyset(&sigact.sa_mask);

        if (sigaction(SIGINT, &sigact, NULL) == -1)
                errExit("sigact");
        
        while (1);
                sleep(1);

        exit(EXIT_SUCCESS);
}
