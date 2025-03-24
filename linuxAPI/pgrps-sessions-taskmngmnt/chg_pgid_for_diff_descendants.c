#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "../essentials.h"

void sigUsr1Handler(int) {
        printf("SIGUSR1 caught.\n");
        fflush(stdout);
}

void sigUsr2Handler(int) {
        printf("SIGUSR2 caught.\n");
        fflush(stdout);
}

int main(void) {
        pid_t childPid1, childPid2;
        struct sigaction sa1, sa2;
        sigset_t blockMask, emptyMask;
        pid_t pgid;
        char buf[128];
        int i;

        sa1.sa_handler = sigUsr1Handler;
        sa1.sa_flags = 0;
        sigemptyset(&sa1.sa_mask);
        if (sigaction(SIGUSR1, &sa1, NULL) == -1)
                errExit("sigaction for SIGUSR1");

        sa2.sa_handler = sigUsr2Handler;
        sa2.sa_flags = 0;
        sigemptyset(&sa2.sa_mask);
        if (sigaction(SIGUSR2, &sa2, NULL) == -1)
                errExit("sigaction for SIGUSR2");
        
        sigemptyset(&blockMask);
        sigaddset(&blockMask, SIGUSR1);
        sigaddset(&blockMask, SIGUSR2);
        if (sigprocmask(SIG_BLOCK, &blockMask, NULL) == -1)
                errExit("sigprocmask");

        printf("Parent  : PID: %ld; PGID: %ld;\n",
                (long) getpid(), (long) getpgrp());

        childPid1 = fork();
        if (childPid1 == -1)
                errExit("fork");
        else if (childPid1 == 0) {
                pgid = getpgrp();
                printf("Children: PID: %ld; PGID: %ld;\n",
                        (long) getpid(), (long) pgid);

                kill(getppid(), SIGUSR1);
                
                _exit(EXIT_SUCCESS);
        }

        childPid2 = fork();
        if (childPid2 == -1)
                errExit("fork");
        else if (childPid2 == 0) {
                pgid = getpgrp();
                printf("Children: PID: %ld; PGID: %ld;\n",
                        (long) getpid(), (long) pgid);
                 
                snprintf(buf, sizeof(buf), "%ld", (long) getppid());
/*                printf("buf value before execl: %s\n", buf); */

                execl("./helper", "helper", buf, (char *) NULL);

                errExit("execl");
        }
        
        sigemptyset(&emptyMask);
        sigsuspend(&emptyMask);

        sigemptyset(&emptyMask);
        sigsuspend(&emptyMask);
        
        sigprocmask(SIG_SETMASK, &emptyMask, NULL);
        
        if (setpgid(childPid1, childPid1) == 0)
                printf("We can change process PGID before exec.\n");
        else
                printf("This is impossible (childPid1).\n");

        if (setpgid(childPid2, childPid2) == -1)
                printf("We can't change process PGID after exec.\n");
        else
                printf("This is impossible (childPid2).\n");

        wait(NULL);
        wait(NULL);
        
        exit(EXIT_SUCCESS);
}
