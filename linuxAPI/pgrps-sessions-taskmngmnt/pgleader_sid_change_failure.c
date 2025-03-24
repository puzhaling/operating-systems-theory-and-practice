#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include "../essentials.h"

int main(int argc, char **argv) {
        if (getpid() == getpgid(0)) {
                if (setsid() == (pid_t) -1)
                        printf("Leader of the process group can't "
                        "create a new session.\n");
                else 
                        printf("Impossible!\n");
                
        } else
                fprintf(stderr, "Program is not a leader of "
                        "current process group\n");

        exit(EXIT_SUCCESS);        
}
