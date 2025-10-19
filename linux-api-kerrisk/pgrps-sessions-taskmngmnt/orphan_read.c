#include "../essentials.h"

int main(void) {
        pid_t pid;
        char buf[128];

        pid = fork();
        if (pid == -1)
                errExit("fork");
        else if (pid == 0) {
                while (getppid() != 1)
                        ;

                if (getppid() == 1)
                        printf("Now we're in orphans process group.\n");
                else
                        printf("Wtf?!\n");

                if (read(STDIN_FILENO, buf, sizeof(buf)) < 0) {
                        if (errno == EIO)
                                printf("read: EIO encountered.\n");
                        else
                                printf("read: error occurred.\n");
                }

                _exit(EXIT_SUCCESS);
        } else
                exit(EXIT_SUCCESS);
}
