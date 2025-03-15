#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "../essentials.h"

static void
usageErr(const char *progname, const char *errMsg)
{
        printf("%s: %s\n", progname, errMsg);
        exit(EXIT_FAILURE);
}

static const 
int procCount = 100000;

int
main(int argc, char *argv[])
{
        if (argc < 2 || strcmp("--help", argv[1]) == 0 || argc > 3)
                usageErr(argv[0], "-D{_FORK,_VFORK} [vmsize]");

        pid_t childPid; 
        int vmSize;
        char *vmAddBlock;
        enum { FORK, VFORK } choice;
        int i;

        vmSize = atoi(argv[2]);
        if (vmSize == 0)
                errExit("atoi");
        vmSize *= 1e6;

        if (strcmp(argv[1], "-D_FORK") == 0)
                choice = FORK;
        else if (strcmp(argv[1], "-D_VFORK") == 0)
                choice = VFORK;
        else
                usageErr(argv[0], "-D{_FORK,_VFORK} [vmsize (MB)]");

        vmAddBlock = (char *) malloc(vmSize);
        if (vmAddBlock == NULL)
                errExit("malloc");

        printf("Total VM size: %s MB\n", argv[2]);
        printf("Number of processes will be created: %d\n", 100000); 

        for (i = 0; i < procCount; ++i) {
                childPid = (choice == FORK) ? fork() : vfork();
                if (childPid == 0)
                        _exit(EXIT_SUCCESS);
                else if (childPid == -1)
                        errExit("fork");
                else
                        wait(NULL);
        }
        
        exit(EXIT_SUCCESS);
}
