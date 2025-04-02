#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "fifo_seqnum.h"
#include "../../essentials.h"

static int serverFd;
static int dummyFd;

void sigHandler(int) {
	write(STDOUT_FILENO, "Server: Bye", 11); 

	close(serverFd);
	close(dummyFd);

	unlink(SERVER_FIFO);

	_exit(EXIT_SUCCESS);
}

int
main(int argc, char **argv) {
        int clientFd, initFileFd;
        char clientFifo[CLIENT_FIFO_NAME_LEN];
        struct request req;
        struct response resp;
        int seqNum;
	ssize_t numRead;

	struct sigaction sa;
	sigset_t blockMask;
         
        umask(0);

	initFileFd = open(SERVER_INIT_FILE, O_RDWR | O_SYNC | O_CREAT,
				S_IRUSR | S_IWUSR);
	if (initFileFd == -1)
		errExit("open SERVER_INIT_FILE");
	
	numRead = read(initFileFd, &seqNum, sizeof(int));
	if (numRead == 0) {
		seqNum = 0;
	} else if (numRead != sizeof(int)) {
		errExit("read SERVER_INIT_FILE");
	}
	
	sa.sa_handler = sigHandler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;

	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
	
        if (mkfifo(SERVER_FIFO, S_IRUSR | S_IWUSR | S_IWGRP) == -1
                        && errno != EEXIST)
                errExit("mkfifo SERVER_FIFO");

        serverFd = open(SERVER_FIFO, O_RDONLY);
        if (serverFd == -1)
                errExit("open SERVER FIFO");

        dummyFd = open(SERVER_FIFO, O_WRONLY);
        if (dummyFd == -1)
                errExit("open SERVER_FIFO");

        if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
                errExit("signal");

        for (;;) {
                if (read(serverFd, &req, sizeof(struct request))
                                != sizeof(struct request)) {
                        fprintf(stderr, "Error reading request; discarding\n");
                        continue;
                }
                
                snprintf(clientFifo, CLIENT_FIFO_NAME_LEN, CLIENT_FIFO_TEMPLATE,
                                (long) req.pid);
                clientFd = open(clientFifo, O_WRONLY | O_NONBLOCK);
                if (clientFd == -1) {
			if (errno == ENXIO)
				fprintf(stderr, "server: client forgot to open "
					"FIFO queue on read");
			else
                        	fprintf(stderr, "open %s", clientFifo);

                        continue;
                }
                
                resp.seqNum = seqNum;
                if (write(clientFd, &resp, sizeof(struct response))
                                != sizeof(struct response))
                        fprintf(stderr, "Error writing to FIFO %s",
                                clientFifo);
                if (close(clientFd) == -1)
                        fprintf(stderr, "close");

                seqNum += req.seqLen;

                if (ftruncate(initFileFd, 0) == -1)
			errExit("ftruncate");
                if (lseek(initFileFd, 0, SEEK_SET) == -1)
			errExit("lseek");
                if (write(initFileFd, &seqNum, sizeof(int)) != sizeof(int))
                        errExit("write SERVER_INIT_FILE");
        }

        exit(EXIT_SUCCESS);
}
