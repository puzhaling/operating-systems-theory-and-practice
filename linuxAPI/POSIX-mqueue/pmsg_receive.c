#include <mqueue.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include "../essentials.h"

static void
usageError(const char *progName) {
	fprintf(stderr, "Usage: %s [-n] [-t TIME] mq-name\n", 
		progName);
	fprintf(stderr, "	-n	Use O_NONBLOCK flag\n");
	fprintf(stderr, "	-t	Use mq_timedreceive (sec)\n");
	exit(EXIT_FAILURE);
}

int
main(int argc, char *argv[]) {
	int flags, opt;
	mqd_t mqd;
	unsigned int prio;
	void *buffer;
	struct mq_attr attr;
	ssize_t numRead;

	Boolean tFlagSet;
	struct timespec delay;
	time_t secs;

	
	tFlagSet = FALSE;
	flags = O_RDONLY;
	while ((opt = getopt(argc, argv, "nt:")) != -1) {
		switch (opt) {
		case 'n': 
			flags |= O_NONBLOCK;
			break;
		case 't': 
			errno = 0;
			secs = atoi(optarg);
			if (secs == 0 && errno != 0)
				errExit("atoi");

			tFlagSet = TRUE;
			break;
		default: 
			usageError(argv[0]);
		}
	}
	
	if (tFlagSet == TRUE) {
		delay.tv_sec = time(NULL) + secs;
		delay.tv_nsec = 0;
	}

	if (optind >= argc)
		usageError(argv[0]);

	mqd = mq_open(argv[optind], flags);
	if (mqd == (mqd_t) -1)
		errExit("mq_open");

	if (mq_getattr(mqd, &attr) == -1)
		errExit("mq_getattr");

	buffer = malloc(attr.mq_msgsize);
	if (buffer == NULL)
		errExit("malloc");
	
	if (tFlagSet) {
	numRead = mq_timedreceive(mqd, buffer, attr.mq_msgsize, &prio, &delay);
	} else {
	numRead = mq_receive(mqd, buffer, attr.mq_msgsize, &prio);
	}

	if (numRead == -1)
		errExit("mq_receive");
	
	printf("Read %ld bytes; priority = %u\n", (long) numRead, prio);
	if (write(STDOUT_FILENO, buffer, numRead) == -1)
		errExit("write");
	write(STDOUT_FILENO, "\n", 1);
	exit(EXIT_SUCCESS);
}
