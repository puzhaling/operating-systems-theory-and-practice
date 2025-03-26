#include "../essentials.h"
#include <sys/resource.h>
#include <sys/wait.h>
#include <sched.h>

int main(int argc, char **argv) {
    if (argc < 4) {
        fprintf(stderr, "%s policy priority command arg...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if ((strcmp(argv[1], "r") != 0) && (strcmp(argv[1], "f") != 0)) {
        fprintf(stderr, "policy must be r(RR) or f(FIFO)\n");
        exit(EXIT_FAILURE);
    }

    int policy = (strcmp("r", argv[1]) == 0) ? SCHED_RR : SCHED_FIFO;
    int priority;
    pid_t pid;
    struct sched_param param;

    errno = 0;
    priority = atoi(argv[2]);
    if (priority == 0 && errno != 0) {
        errExit("NaN priority value");
    }

    int priomin = sched_get_priority_min(policy);
    int priomax = sched_get_priority_max(policy);
    if (priority < priomin || priority > priomax) {
        fprintf(stderr, "priority value is out of range (min=%d, max=%d)\n", 
                priomin, priomax);
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == -1) {
        errExit("fork");
    } else if (pid == 0) {
        if (seteuid(0) == -1) {
            perror("seteuid");
            _exit(EXIT_FAILURE);
        }

        param.sched_priority = priority;
        if (sched_setscheduler(0, policy, &param) == -1) {
            perror("sched_setscheduler");
            _exit(EXIT_FAILURE);
        }

        if (seteuid(getuid()) == -1) {
            perror("seteuid");
            _exit(EXIT_FAILURE);
        }

        if (execvp(argv[3], argv + 3) == -1) {
            perror("execvp");
            _exit(EXIT_FAILURE);
        }

        _exit(EXIT_FAILURE);
    }

    if (waitpid(pid, NULL, 0) == -1) {
        errExit("waitpid");
    }

    exit(EXIT_SUCCESS);
}
