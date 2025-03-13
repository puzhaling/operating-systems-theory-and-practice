#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include "../essentials.h"

int main(void)
{
    switch (fork())
        {
        case -1:
            errExit("fork");
            break;
        case 0:
            printf("PID: %ld (child process);" 
                   "PID: %ld (parent process)\n", 
                   (long) getpid(), 
                   (long) getppid());
            
            if (kill(getppid(), SIGTERM) == -1)
                errExit("kill on parent");
            
            /* wait for parent's death */
            sleep(3);

            printf("parent was killed\n");
             
            printf("PID: %ld (new parent process)\n",
                   (long) getppid());

            _exit(EXIT_SUCCESS);
            break;
        default:
            pause();
            break;
        }
    
    exit(EXIT_SUCCESS);
}
