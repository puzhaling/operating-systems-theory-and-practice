#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include "../essentials.h"

int main(void) {
	int fifoReadFd, fifoWriteFd;
	mode_t fifoMode;

	ssize_t nread, nwrite;
	char buf[10];

	printf("Demonstrating of non-blocking "
	"IO in FIFO queue context.\n\n");
	
	fifoMode = S_IRUSR | S_IWUSR;
	if (mkfifo("myfifo", fifoMode) == -1 && errno != EEXIST)
		errExit("mkfifo");
	
	fifoReadFd = open("myfifo", O_NONBLOCK | O_RDONLY);
	if (fifoReadFd == -1)
		errExit("open read end of FIFO");

	printf("Just now I opened FIFO queue in non-blocking mode for READ. "
	"If flag O_NONBLOCK had not been provided, this program would "
	"have hung.\n\n");

	printf("Trying to READ smth from opened FIFO. "
	"If this is the last message you see, then "
	"non-blocking mode is not working :'(\n\n");

	nread = read(fifoReadFd, buf, sizeof(buf));
	if (nread == -1)
		errExit("read");
	else
		printf("Ooof, read(2) returned and read %d bytes.\n\n",
			(int) nread);
	
	fifoWriteFd = open("myfifo", O_NONBLOCK | O_WRONLY);
	if (fifoWriteFd == -1)
		errExit("open write end of FIFO");

	printf("Just now I opened FIFO queue in non-blocking mode for WRITE. "
	"If flag O_NONBLOCK had not been provided, this program would "
	"have hung.\n\n");

	printf("Trying to WRITE smth from opened FIFO. "
	"If this is the last message you see, then "
	"non-blocking mode is not working :'(\n\n");

	nwrite = write(fifoWriteFd, "a", 1);
	if (nwrite == -1)
		errExit("write");
	else
		printf("Ooof, write(2) returned and write %d bytes "
		"(\"a\" string)\n\n",
			(int) nwrite);
	
	if (unlink("myfifo") == -1)
		warn("unlink myfifo");

	exit(EXIT_SUCCESS);	
}
