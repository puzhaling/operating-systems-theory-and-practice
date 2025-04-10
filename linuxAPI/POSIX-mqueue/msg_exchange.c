#include <mqueue.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <signal.h>
#include "../essentials.h"

static mqd_t mqd1, mqd2;
static char q1_name[100], q2_name[100];
static char *inMsg, *outMsg;

static void
usageError(const char *progname) {
	fprintf(stderr, 
		"Usage: %s USER-FROM USER-TO\n",
		progname);

	exit(EXIT_FAILURE);
}

void cleaner(int) {
	mq_close(mqd1);
	mq_close(mqd2);

	if (mq_unlink(q1_name) == -1 && errno != ENOENT)
		errExit("mq_unlink: q1_name");
	if (mq_unlink(q2_name) == -1 && errno != ENOENT)
		errExit("mq_unlink: q2_name");
	
	free(inMsg);
	free(outMsg);

	exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
	if (argc != 3)
		usageError(argv[0]);

	mode_t mqMode;
	struct mq_attr attr;
	char *rp;
	struct sigaction sa;

	sa.sa_handler = cleaner;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;

	if (sigaction(SIGINT, &sa, NULL) == -1)
		errExit("sigaction: SIGINT");

	snprintf(q1_name, 100, "/%s_to_%s", argv[1], argv[2]);
	snprintf(q2_name, 100, "/%s_to_%s", argv[2], argv[1]);

	mqMode = S_IRUSR | S_IWUSR | S_IROTH | S_IWOTH;
	mqd1 = mq_open(q1_name, O_RDWR | O_CREAT, mqMode, NULL);
	if (mqd1 == (mqd_t) -1)
		errExit("mq_open: mqd1");
	mqd2 = mq_open(q2_name, O_RDWR | O_CREAT, mqMode, NULL);
	if (mqd2 == (mqd_t) -1)
		errExit("mq_open: mqd2");
	
	if (mq_getattr(mqd1, &attr) == -1)
		errExit("mq_getattr");
	
	inMsg = malloc(attr.mq_msgsize);
	if (inMsg == NULL)
		errExit("malloc: inMsg:");
	outMsg = malloc(attr.mq_msgsize);
	if (outMsg == NULL)
		errExit("malloc: outMsg:");
	
	switch (fork()) {
	case -1:
		errExit("fork");
	case 0:
		if (mq_close(mqd1) == -1)
			errExit("child: mq_close: mqd1");
		for (;;) {
			if (mq_receive(mqd2, inMsg, attr.mq_msgsize, 0) == -1)
				errExit("child: mqd2: mq_receive");

			fprintf(stdout, "\n%s\n", inMsg);
		}
	default:
		if (mq_close(mqd2) == -1)
			errExit("parent: mq_close: mqd2");
		for (;;) {
			rp = fgets(outMsg, attr.mq_msgsize, stdin);
			if (rp == NULL)
				errExit("child: fgets");

			if (mq_send(mqd1, outMsg, attr.mq_msgsize, 0) == -1)
				errExit("parent: mqd1: mq_send");
		}
	}
}
