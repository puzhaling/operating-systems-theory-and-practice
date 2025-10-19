#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t parent_pid, child_pid;

    parent_pid = fork();

    if (parent_pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (parent_pid == 0) {
        child_pid = fork();

        if (child_pid < 0) {
            perror("fork");
            _exit(EXIT_FAILURE);
        } else if (child_pid == 0) {
            printf("Son: PID = %d; PPID = %d\n", getpid(), getppid());
            sleep(2);
            printf("Son: After father's death: PPID = %d\n", getppid());
            _exit(EXIT_SUCCESS);
        } else {
            printf("Father: PID = %d; PPID = %d\n", getpid(), getppid());
            printf("Father: I'm dying...\n");
            _exit(EXIT_SUCCESS);
        }
    } else {
        sleep(1); 
        printf("Grandfather: I'm not calling wait()\n");
        sleep(2);
    }

    exit(EXIT_SUCCESS);
}
