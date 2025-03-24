#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include "../essentials.h"

int 
main(int argc, char **argv) {
        pid_t pid;

        pid = atol(argv[1]);
        printf("helper pid arg = %ld\n", (long) pid);
        if (kill(pid, SIGUSR2) == -1)
                errExit("kill");
/*        else
                printf("handler: SIGUSR2 sent successfully.\n"); */
        

        exit(EXIT_SUCCESS);
}
