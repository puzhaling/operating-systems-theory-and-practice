#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../essentials.h"

static pthread_mutex_t thrdFunc1_mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t thrdFunc2_mtx = PTHREAD_MUTEX_INITIALIZER;

static pthread_cond_t thrdFunc1_block_cond = PTHREAD_COND_INITIALIZER;
static pthread_cond_t thrdFunc2_block_cond = PTHREAD_COND_INITIALIZER;
static pthread_cond_t thrdFunc1_sig_cond   = PTHREAD_COND_INITIALIZER;
static pthread_cond_t thrdFunc2_sig_cond   = PTHREAD_COND_INITIALIZER;

static Boolean thrd1_SigBlockCompleted = FALSE;
static Boolean thrd1_SigSent           = FALSE;
static Boolean thrd2_SigBlockCompleted = FALSE;
static Boolean thrd2_SigSent           = FALSE;

void *thrdFunc1(void *arg) {
        sigset_t blockMask, pendingMask;
        int signum;

        sigemptyset(&blockMask);
        sigaddset(&blockMask, SIGUSR1);
        pthread_sigmask(SIG_SETMASK, &blockMask, NULL);

        pthread_mutex_lock(&thrdFunc1_mtx);
        thrd1_SigBlockCompleted = TRUE;
        pthread_cond_signal(&thrdFunc1_block_cond);
        pthread_mutex_unlock(&thrdFunc1_mtx);

        pthread_mutex_lock(&thrdFunc1_mtx);
        while (thrd1_SigSent == FALSE) {
                pthread_cond_wait(&thrdFunc1_sig_cond, &thrdFunc1_mtx);
        }
        pthread_mutex_unlock(&thrdFunc1_mtx);

        sigpending(&pendingMask);
        printSigset(stdout, "thrdFunc1 pending signal mask: ", &pendingMask);

        pthread_exit(NULL);
}

void *thrdFunc2(void *arg) {
        sigset_t blockMask, pendingMask;
        int signum;

        sigemptyset(&blockMask);
        sigaddset(&blockMask, SIGUSR2);
        pthread_sigmask(SIG_SETMASK, &blockMask, NULL);

        pthread_mutex_lock(&thrdFunc2_mtx);
        thrd2_SigBlockCompleted = TRUE;
        pthread_cond_signal(&thrdFunc2_block_cond);
        pthread_mutex_unlock(&thrdFunc2_mtx);

        pthread_mutex_lock(&thrdFunc2_mtx);
        while (thrd2_SigSent == FALSE) {
                pthread_cond_wait(&thrdFunc2_sig_cond, &thrdFunc2_mtx);
        }
        pthread_mutex_unlock(&thrdFunc2_mtx);

        sigpending(&pendingMask);
        printSigset(stdout, "thrdFunc2 pending signal mask: ", &pendingMask);

        pthread_exit(NULL);
}

int main(void) {
        pthread_t t1, t2;
        int rc;

        pthread_create(&t1, NULL, thrdFunc1, NULL);
        pthread_create(&t2, NULL, thrdFunc2, NULL);

        pthread_mutex_lock(&thrdFunc1_mtx);
        while (thrd1_SigBlockCompleted == FALSE) {
                pthread_cond_wait(&thrdFunc1_block_cond, &thrdFunc1_mtx);
        }
        pthread_mutex_unlock(&thrdFunc1_mtx);

        pthread_mutex_lock(&thrdFunc2_mtx);
        while (thrd2_SigBlockCompleted == FALSE) {
                pthread_cond_wait(&thrdFunc2_block_cond, &thrdFunc2_mtx);
        }
        pthread_mutex_unlock(&thrdFunc2_mtx);

        pthread_kill(t1, SIGUSR1);
        pthread_kill(t2, SIGUSR2);

        pthread_mutex_lock(&thrdFunc1_mtx);
        thrd1_SigSent = TRUE;
        pthread_cond_signal(&thrdFunc1_sig_cond);
        pthread_mutex_unlock(&thrdFunc1_mtx);

        pthread_mutex_lock(&thrdFunc2_mtx);
        thrd2_SigSent = TRUE;
        pthread_cond_signal(&thrdFunc2_sig_cond);
        pthread_mutex_unlock(&thrdFunc2_mtx);

        pthread_join(t1, NULL);
        pthread_join(t2, NULL);

        exit(EXIT_SUCCESS);
}
