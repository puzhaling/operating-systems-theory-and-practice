#include "inclme.h"

int
Socket(int family, int type, int protocol) {
	int	n;

	if ( (n = socket(family, type, protocol)) < 0)
		err_quit("socket error");
	return 	n;
}

int Bind(int sockfd, const struct sockaddr *addr,
	socklen_t addrlen)
{
	if (bind(sockfd, addr, addrlen) == -1)
		err_quit("bind error");
	return 0;
}

int Listen(int sockfd, int backlog) {
	if (listen(sockfd, backlog) == -1)
		err_quit("listen error");
	return 0;
}

int Accept(int sockfd, struct sockaddr *addr,
	socklen_t *addrlen)
{
	int	n;

	if ( (n = accept(sockfd, addr, addrlen)) < 0)
		err_quit("accept error");
	return	n;
}

void
Pthread_mutex_lock(pthread_mutex_t *mptr) {
	int	n;

	if ( (n = pthread_mutex_lock(mptr)) == 0)
		return;
	errno = n;
	err_quit("pthread_mutex_lock error");
}

int Close(int fd) {
	if (close(fd) == -1)
		err_quit("close error");
	return 0;
}

ssize_t Read(int fd, void *buf, size_t count) {
	int	n;

	if ( (n = read(fd, buf, count)) < 0)
		err_quit("read error");
	return 	n;
}

ssize_t Write(int fd, const void *buf, size_t count) {
	int	n;

	if ( (n = write(fd, buf, count)) == -1)
		err_quit("write error");
	return 	n;
}
