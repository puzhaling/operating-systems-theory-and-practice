#include <signal.h>
#include "../essentials.h"

void sig_handler(int signum) {
        const char* msg = NULL;
        switch(signum) {
                case SIGTTIN: msg = "SIGTTIN handled\n"; break;
                case SIGTTOU: msg = "SIGTTOU handled\n"; break;
                case SIGTSTP: msg = "SIGTSTP handled\n"; break;
        }
        if (msg) write(STDOUT_FILENO, msg, strlen(msg));
}

int main(void) {
        pid_t pid = fork();
        
        if (pid == -1) {
                errExit("fork");
        } else if (pid == 0) {
                while(getppid() != 1)
                        sleep(1);
                
                printf("Child: Now I'm an orphan (PPID=%d)\n", getppid());
                
                signal(SIGTTIN, SIG_DFL);
                signal(SIGTTOU, SIG_DFL);
                signal(SIGTSTP, SIG_DFL);
                
                printf("Sending signals with SIG_DFL...\n");
                raise(SIGTTIN);
                raise(SIGTTOU);
                raise(SIGTSTP);
                printf("Still alive after SIG_DFL signals!\n\n");
                
                signal(SIGTTIN, sig_handler);
                signal(SIGTTOU, sig_handler);
                signal(SIGTSTP, sig_handler);
                
                printf("Sending signals with handlers...\n");
                raise(SIGTTIN);
                raise(SIGTTOU);
                raise(SIGTSTP);
                
                _exit(EXIT_SUCCESS);
        }
        
        exit(EXIT_SUCCESS);
}
