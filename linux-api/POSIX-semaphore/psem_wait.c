#include <semaphore.h>
#include <string.h>
#include <time.h>
#include "../essentials_tlpi.h"

int
main(int argc, char *argv[])
{
	sem_t *sem;
	struct timespec delay;

	if (argc != 2 || argc != 3 || strcmp(argv[1], "--help") == 0)
		usageErr("%s [TIME] sem-name\n", argv[0]);


	sem = sem_open(argv[1], 0);
	if (sem == SEM_FAILED)
		errExit("sem_open");
	
	delay.tv_nsec = 0;
	delay.tv_sec = time(NULL) + (atoi(argv[2]) * 1e6);
	if (sem_timedwait(sem, &delay) == -1)
		errExit("sem_wait");

	printf("%ld sem_wait() succeeded\n", (long) getpid());
	exit(EXIT_SUCCESS);
}
