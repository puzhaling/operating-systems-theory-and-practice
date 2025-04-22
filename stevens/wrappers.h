#ifndef WRAPPERS_H
#define WRAPPERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <netdb.h>
#include <pthread.h>

int Socket(int family, int type, int protocol);
int Bind(int sockfd, const struct sockaddr *addr,
	socklen_t addrlen);
int Listen(int sockfd, int backlog);
int Accept(int sockfd, struct sockaddr *addr,
	socklen_t *addrlen);


void Pthread_mutex_lock(pthread_mutex_t *mptr);

int Close(int fd);
ssize_t Read(int fd, void *buf, size_t count);
ssize_t Write(int fd, const void *buf, size_t count);

#endif /* WRAPPERS_H */
