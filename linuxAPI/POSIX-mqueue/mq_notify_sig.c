#include <signal.h>
#include <errno.h>
#include <stdlib.h>
#include <mqueue.h>
#include <fcntl.h>
#include <string.h>
#include "../essentials.h"

#define NOTIFY_SIG SIGUSR1

static void
handler(int sig) {
	printf("mq_notify_sig: notify sig\n");
}

int main(int argc, char *argv[]) {
	struct sigevent sev;
	Boolean singleNotify;
	mqd_t mqd;
	struct mq_attr attr;
	void *buffer;
	ssize_t numRead;
	sigset_t blockMask,
		emptyMask;
	struct sigaction sa;

	if (argc != 2 || strcmp(argv[1], "--help")
		== 0) {
		fprintf(stderr, "%s mq-name\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	mqd = mq_open(argv[1], O_RDONLY | O_NONBLOCK);
	if (mqd == (mqd_t) -1)
		errExit("mq_open");
	
	if (mq_getattr(mqd, &attr) == -1)
		errExit("mq_getattr");

	buffer = malloc(attr.mq_msgsize);
	if (buffer == NULL)
		errExit("malloc");

	sigemptyset(&blockMask);
	sigaddset(&blockMask, NOTIFY_SIG);
	if (sigprocmask(SIG_BLOCK, &blockMask, NULL) == -1)
		errExit("sigprocmask");
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = handler;
	if (sigaction(NOTIFY_SIG, &sa, NULL) == -1)
		errExit("sigaction");

	sev.sigev_notify = SIGEV_SIGNAL;
	sev.sigev_signo = NOTIFY_SIG;
	if (mq_notify(mqd, &sev) == -1)
		errExit("mq_notify");

	sigemptyset(&emptyMask);
	
	singleNotify = FALSE;
	for (;;) {
		sigsuspend(&emptyMask);

		if (singleNotify == FALSE)
		    if (mq_notify(mqd, &sev) == -1)
			    errExit("mq_notify");

		singleNotify = TRUE;

		errExit("mq_notify");
		printf("mq_notify_sig: Reading from queue...\n");
		while ((numRead = mq_receive(mqd, buffer, 
			attr.mq_msgsize, NULL)) >= 0)
			printf("mq_notify_sig: Read %ld bytes\n", (long) numRead);
		
		if (errno != EAGAIN)
			errExit("mq_receive");
	}
}
