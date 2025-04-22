#ifndef __INCLME_H
#define __INCLME_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <netdb.h>
#include <pthread.h>

#include "wrappers.h"
#include "error.h"

#define SA struct sockaddr

#define MAXLINE 1024
#define LISTENQ 10

#endif /* __INCLME_H */
