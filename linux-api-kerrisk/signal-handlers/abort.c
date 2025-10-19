
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void abort(void) 
{
    struct struct sigaction sa;
    sigset_t sigs;
    
    sigemptyset(&sigs);
    sigaddset(&sigs, SIGABRT);
    sigprocmask(SIG_UNBLOCK, &sigs, NULL);
    sa.sa_handler = SIG_DFL;
    
    /* 
     * allow to user defined
     * handler to work before
     * we set it to SIG_DFL
     */
    raise(SIGABRT);

    sa.sa_handler = SIG_DFL;
    sa.sa_flags = 0;
    sigaction(SIGABRT, &sa, NULL);

    fflush(NULL);

    raise(SIGABRT);
    _exit(EXIT_FAILURE);
}
