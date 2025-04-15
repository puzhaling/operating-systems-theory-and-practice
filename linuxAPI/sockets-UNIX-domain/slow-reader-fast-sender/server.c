#include "hdr.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

int main(void) {
	int sockfd;
	struct sockaddr_un addr;
	char msg[BUF_SIZE];
	int numBytes;

	sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
	if (sockfd == -1)
		handle_error("socket");

	if (remove(sockpath) == -1 && errno != ENOENT)
		handle_error("remove");
	
	memset(&addr, 0, sizeof(struct sockaddr));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, sockpath, strlen(sockpath) - 1);

	if (bind(sockfd, (struct sockaddr *) &addr,
		sizeof(struct sockaddr_un)) == -1)
		handle_error("bind");
	
	printf("server: waiting for messages...\n");
	while (1) {
		sleep(3);

		numBytes = recvfrom(sockfd, msg, BUF_SIZE, 0, NULL, NULL);
		if (numBytes == -1)
			handle_error("recvfrom");
		
		printf("server: recieved %d bytes\n", numBytes);
	}
}
