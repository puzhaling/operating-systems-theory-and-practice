#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "../essentials.h"

void 
printProcResUsageInfo(const struct rusage *usage)
{
        printf("utime (secs): %ld\n", 
                (long) usage->ru_utime.tv_sec);
        printf("stime (secs): %ld\n", 
                (long) usage->ru_stime.tv_sec);
        printf("utime (microsecs): %ld\n", 
                (long) usage->ru_utime.tv_usec);
        printf("stime (microsecs): %ld\n", 
                (long) usage->ru_stime.tv_usec);
}
int main(int argc, char **argv) {
        if (argc < 2)
                cmdLineErr("not enough arguments");
        
        struct rusage rusage;
        pid_t         pid;

        pid = fork();
        if (pid == -1)
                errExit("fork");
        else if (pid == 0) {
                if (execvp(argv[1], argv + 1) == -1)
                        perror("execvp");

                _exit(EXIT_FAILURE);
        }

        if (waitpid(pid, NULL, 0) == -1)
                errExit("waitpid");
        
        getrusage(RUSAGE_CHILDREN, &rusage);
        
        printProcResUsageInfo(&rusage);
                
        exit(EXIT_SUCCESS);
}
