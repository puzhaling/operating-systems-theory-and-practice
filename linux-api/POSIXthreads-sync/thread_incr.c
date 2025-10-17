#define _GNU_SOURCE
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include "../essentials.h"

static int threadID = 0;
static int glob = 0;

typedef struct __threadFuncArg {
        int id;
        int loops;
} threadFuncArg;

static void *
threadFunc(void *arg)
{
        int loops = ((threadFuncArg *) arg)->loops;
        int id = ((threadFuncArg *) arg)->id; 
        int loc, j;

        for (j = 0; j < loops; ++j) {
                printf("glob = %d (ID = %d)\n", glob, id);

                loc = glob;
                loc++;
                glob = loc;
        }

        return NULL;
}

int
main(int argc, char **argv)
{
        pthread_t t1, t2;
        threadFuncArg arg1, arg2;
        int s;
        int loops;

        loops = (argc > 1) ? atoi(argv[1]) : 10000000;

        arg1.loops = loops;
        arg1.id = threadID++;
        arg2.loops = loops;
        arg2.id = threadID++;

        s = pthread_create(&t1, NULL, threadFunc, &arg1);
        if (s != 0)
                errExitEN(s, "pthread_create");
        
        s = pthread_create(&t2, NULL, threadFunc, &arg2);
        if (s != 0)
                errExitEN(s, "pthread_create");

        s = pthread_join(t1, NULL);
        if (s != 0)
                errExitEN(s, "pthread_create");

        s = pthread_join(t2, NULL);
        if (s != 0)
                errExitEN(s, "pthread_create");

        printf("glob = %d\n", glob);
        exit(EXIT_SUCCESS);
}
