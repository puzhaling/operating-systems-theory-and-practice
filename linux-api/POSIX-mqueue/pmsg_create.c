#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "../essentials.h"

int
main(int argc, char *argv[]) {
	mqd_t mqd;

	mqd = mq_open("/mq", O_CREAT, S_IRUSR | S_IWUSR);
	if (mqd == (mqd_t) -1)
		exit(EXIT_FAILURE);

	exit(EXIT_SUCCESS);
}
