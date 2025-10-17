#include <sys/mman.h>
#include <sys/resource.h>
#include <stdio.h>
#include <stdlib.h>

#define PAGE_SIZE 4096

int main(void) {
	struct rlimit rlim;	
	void *addr;

	if (getrlimit(RLIMIT_MEMLOCK, &rlim) == -1) {
		perror("getrlim RLIMIT_MEMLOCK");
		exit(EXIT_FAILURE);
	}
	
	printf("(before setrlim) RLIMIT_MEMLOCK soft = %ld\n", 
		(long) rlim.rlim_cur);
	printf("(before setrlim) RLIMIT_MEMLOCK hard = %ld\n", 
		(long) rlim.rlim_max);
	
	rlim.rlim_cur = PAGE_SIZE;
	if (setrlimit(RLIMIT_MEMLOCK, &rlim) == -1) {
		perror("getrlim RLIMIT_MEMLOCK");
		exit(EXIT_FAILURE);
	}

	if (getrlimit(RLIMIT_MEMLOCK, &rlim) == -1) {
		perror("getrlim RLIMIT_MEMLOCK");
		exit(EXIT_FAILURE);
	}
	
	printf("(after setrlim) RLIMIT_MEMLOCK soft = %ld\n", 
		(long) rlim.rlim_cur);
	printf("(after setrlim) RLIMIT_MEMLOCK hard = %ld\n", 
		(long) rlim.rlim_max);
	
	printf("Trying to lock more memory (%d), " 
		"than rlimit allow (%d)\n", (int) PAGE_SIZE * 2, rlim.rlim_cur);

	addr = malloc(PAGE_SIZE * 2);
	if (mlock(addr, PAGE_SIZE * 2) == -1) {
		perror("mlock");
		exit(EXIT_FAILURE);
	}
	
	exit(EXIT_SUCCESS);
}
