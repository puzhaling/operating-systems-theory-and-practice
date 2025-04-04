#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

void sigBusHandler(int) {
	write(STDOUT_FILENO, "SIGBUS caught\n", 14);
	_exit(1);
}

void sigSegvHandler(int) {
	write(STDOUT_FILENO, "SIGSEGV caught\n", 15);
	_exit(1);
}

int
main(int argc, char *argv[]) {
	void *addr;
	int fd;
	char c;
	off_t fileSiz;
	struct stat st;
	struct sigaction saBus, saSegv;
	
	fd = open("testfile", O_RDONLY);
	if (fd == -1) {
		perror("open");
		exit(EXIT_FAILURE);
	}

	if (fstat(fd, &st) == -1) {
		perror("fstat");
		exit(EXIT_FAILURE);
	}
	fileSiz = st.st_size;

	saBus.sa_handler = sigBusHandler;
	sigemptyset(&saBus.sa_mask);
	saBus.sa_flags = 0;
	if (sigaction(SIGBUS, &saBus, NULL) == -1) {
		perror("sigaction");
		exit(EXIT_FAILURE);
	}

	saSegv.sa_handler = sigSegvHandler;
	sigemptyset(&saSegv.sa_mask);
	saSegv.sa_flags = 0;
	if (sigaction(SIGSEGV, &saSegv, NULL) == -1) {
		perror("sigaction");
		exit(EXIT_FAILURE);
	}

	addr = mmap(NULL, 8192, PROT_READ, 
		MAP_PRIVATE, fd, 0);
	if (addr == MAP_FAILED) {
		perror("mmap");
		exit(EXIT_FAILURE);
	}
	close(fd);
	
	// ZERO access
	c = *((char *) addr + 300);
	// SIGBUS generating
	c = *((char *) addr + 5000);
	// SUGSEGV generating
	// c = *((char *) addr + 9000);
	
	if (munmap(addr, 8192) == -1) {
		perror("munmap");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}
