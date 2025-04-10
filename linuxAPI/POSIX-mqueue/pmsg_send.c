#include <mqueue.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include "../essentials.h"

int
main(int argc, char *argv[]) {
	mqd_t mqd;
	
	mqd = mq_open("/mq", O_WRONLY);
	if (mqd == (mqd_t) -1)
		errExit("mq_open");

	if (mq_send(mqd, "message", strlen("message"), 0) == -1)
		errExit("mq_send");
	
	exit(EXIT_SUCCESS);
}
