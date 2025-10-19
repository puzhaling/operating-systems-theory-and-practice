#include <sys/resource.h>
#include <sys/wait.h>
#include "../essentials.h"

int
main(int argc, char **argv) {
        if (argc < 2 || (strcmp("--help", argv[1])) == 0) {
                fprintf(stderr, "%s [OPTION] COMMAND [ARG]...\n", argv[0]);
                exit(EXIT_FAILURE);
        }

        int opt;
        pid_t pid;
        int niceInc;
        int i;
        
        niceInc = 10;
        while ((opt = getopt(argc, argv, "n:")) != -1) {
                switch (opt) {
                case 'n':
                        errno = 0;
                        niceInc = atoi(optarg);

                        if (niceInc == 0 && errno != 0)
                                niceInc = 10;
                        
                        break;
                default: /* '?' */
                        fprintf(stderr, "%s: invalid flag\n", argv[0]);
                        exit(EXIT_FAILURE);
                }
        }
        
        if (argv[optind] == NULL) {
                fprintf(stderr, "%s [OPTION] COMMAND [ARG]...\n", argv[0]);
                exit(EXIT_FAILURE);
        }
        
        pid = fork();
        if (pid == -1) {
                perror("fork");
                exit(EXIT_FAILURE);
        } else if (pid == 0) {
                if (nice(niceInc) == -1) 
                        perror("nice");
                
                printf("Child's nice value: %d\n", getpriority(PRIO_PROCESS, 0));
                printf("optind = %d, argv[optind] = %s\n", optind, argv[optind]);

                if (execvp(argv[optind], argv + optind) == -1)
                        perror("execv");
                
                _exit(EXIT_FAILURE);
        }

        pid = waitpid(pid, NULL, 0); 
        if (pid == -1) {
                perror("waitpid");
                exit(EXIT_FAILURE);
        }
        
        exit(EXIT_SUCCESS);
}
