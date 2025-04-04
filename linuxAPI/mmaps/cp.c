#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

int
main(int argc, char *argv[]) {
	if (argc != 3) {
		fprintf(stderr, "usage: %s SOURCE DEST\n",
			argv[0]);
		exit(EXIT_FAILURE);	
	}

	int srcFd, dstFd;
	struct stat st;
	off_t srcSiz;
	void *saddr, *daddr;
		
	if ((srcFd = open(argv[1], O_RDWR)) == -1) {
		perror("open");
		exit(EXIT_FAILURE);
	}
	
	if ((dstFd = open(argv[2], O_RDWR)) == -1) {
		perror("open");
		exit(EXIT_FAILURE);
	}

	if (fstat(srcFd, &st) == -1) {
		perror("fstat");
		exit(EXIT_FAILURE);
	}
	srcSiz = st.st_size;
	
	saddr = mmap(NULL, (size_t) srcSiz, PROT_READ, MAP_PRIVATE, srcFd, 0);
	if (saddr == MAP_FAILED) {
		perror("mmap");
		exit(EXIT_FAILURE);
	}
	if (close(srcFd) == -1) {
		perror("close");
		exit(EXIT_FAILURE);
	}

	if (ftruncate(dstFd, srcSiz) == -1) {
		perror("ftruncate");
		exit(EXIT_FAILURE);
	}
	daddr = mmap(NULL, (size_t) srcSiz, PROT_WRITE, MAP_SHARED, dstFd, 0);
	if (daddr == MAP_FAILED) {
		perror("mmap");
		exit(EXIT_FAILURE);
	}
	if (close(dstFd) == -1) {
		perror("close");
		exit(EXIT_FAILURE);
	}
	
	printf("before memcpy: daddr[0] = %c\n", *((char *) daddr));
	printf("before memcpy: daddr[1] = %c\n", *((char *) daddr + 1));
	printf("before memcpy: daddr[2] = %c\n", *((char *) daddr + 2));
	memcpy(daddr, saddr, srcSiz);
	printf("after memcpy: daddr[0] = %c\n", *((char *) daddr));
	printf("after memcpy: daddr[1] = %c\n", *((char *) daddr + 1));
	printf("after memcpy: daddr[2] = %c\n", *((char *) daddr + 2));

	
	if (msync(daddr, srcSiz, MS_SYNC) == -1) {
		perror("msync");
		exit(EXIT_FAILURE);
	}

	if (munmap(saddr, srcSiz) == -1) {
		perror("munmap");
		exit(EXIT_FAILURE);
	}
	if (munmap(daddr, srcSiz) == -1) {
		perror("munmap");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}
