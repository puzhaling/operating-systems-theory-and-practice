#include "fifo_seqnum.h"
#include "../../essentials.h"
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

static char clientFifo[CLIENT_FIFO_NAME_LEN];

static
void removeFifo(void) {
        unlink(clientFifo);
}

int
main(int argc, char *argv[]) {
        int serverFd, clientFd;
        struct request req;
        struct response resp;
        
        if (argc > 1 && strcmp(argv[1], "--help") == 0)
                usageErr("program [seq-len...]\n");

        umask(0);
        snprintf(clientFifo, CLIENT_FIFO_NAME_LEN, CLIENT_FIFO_TEMPLATE,
                        (long) getpid());
        if (mkfifo(clientFifo, S_IRUSR | S_IWUSR | S_IWGRP) == -1
                        && errno != EEXIST)
                errExit("mkfifo clientFifo");

        if (atexit(removeFifo) != 0)
                errExit("atexit");

        req.pid = getpid();

        errno = 0;
        req.seqLen = (argc > 1) ? atoi(argv[1]) : 1;
        if (req.seqLen == 0 && errno != 0)
                req.seqLen = 1;

        serverFd = open(SERVER_FIFO, O_WRONLY);
        if (serverFd == -1)
                errExit("open SERVER_FIFO");

        if (write(serverFd, &req, sizeof(struct request)) !=
                        sizeof(struct request))
                errExit("Can't write to server");

        clientFd = open(clientFifo, O_RDONLY);
        if (clientFd == -1)
                errExit("open clientFifo");

        if (read(clientFd, &resp, sizeof(struct response))
                        != sizeof(struct response))
                errExit("Can't read response from server");
	
	close(serverFd);
	close(clientFd);

        printf("%d\n", resp.seqNum);

        exit(EXIT_SUCCESS);
}
