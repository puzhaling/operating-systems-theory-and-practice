#include "hdr.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define BACKLOG 5
#define handle_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while (0)

int
main(int argc, char *argv[])
{
	struct sockaddr_un addr;
	int sfd, cfd;
	ssize_t numRead;
	char buf[BUF_SIZE];

	memset(&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	strncpy(&addr.sun_path[1], abstrSocketName, sizeof(addr.sun_path) - 2);

	sfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sfd == -1)
		handle_error("socket");

	if (bind(sfd, (struct sockaddr *) &addr,
		sizeof(sa_family_t) + strlen(abstrSocketName) + 1) == -1)
		handle_error("bind");

	if (listen(sfd, BACKLOG) == -1)
		handle_error("listen");

	for (;;) {
		cfd = accept(sfd, NULL, NULL);
		if (cfd == -1)
			handle_error("accept");

		while ((numRead = read(cfd, buf, BUF_SIZE)) > 0)
			if (write(STDOUT_FILENO, buf, numRead) != numRead)
				handle_error("partial/failed write");

		if (numRead == -1)
			handle_error("read");

		if (close(cfd) == -1)
			handle_error("close");
	}
}
