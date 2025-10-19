#include "hdr.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/un.h>
#include <stdlib.h>
#include <unistd.h>

#define BACKLOG 5
#define handle_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while (0)

int
main(int argc, char *argv[])
{
	struct sockaddr_un addr;
	int sfd;
	ssize_t numRead;
	char buf[BUF_SIZE];

	sfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sfd == -1)
		handle_error("socket");

	memset(&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	strncpy(&addr.sun_path[1], abstrSocketName, sizeof(addr.sun_path) - 2);

	if (connect(sfd, (struct sockaddr *) &addr,
		sizeof(sa_family_t) + strlen(abstrSocketName) + 1) == -1)
		handle_error("connect");

	while ((numRead = read(STDOUT_FILENO, buf, BUF_SIZE)) > 0)
		if (write(sfd, buf, numRead) != numRead)
			handle_error("partial/failed write");

	if (numRead == -1)
		handle_error("read");

	exit(EXIT_SUCCESS);
}
