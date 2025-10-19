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
	const char *msg = "I LOVE GREEN APPLES AND LONG MESSAGES";
	int numBytes;

	sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
	if (sockfd == -1)
		handle_error("socket");
	
	memset(&addr, 0, sizeof(struct sockaddr));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, sockpath, strlen(sockpath) - 1);
	
	printf("client: sending messages...\n");
	while (1) {
		numBytes = sendto(sockfd, msg, strlen(msg), 0, 
			(struct sockaddr *) &addr, 
			sizeof(struct sockaddr_un));
		if (numBytes == -1)
			handle_error("sendto");
		
		printf("client: sent %d bytes\n", numBytes);
	}
}
