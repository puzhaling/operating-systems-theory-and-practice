#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

typedef struct __barrier_t {
    sem_t sem;
    sem_t lock;
    int threshold;
    int waiters;
} barrier_t;

barrier_t b;

void barrier_init(barrier_t *b, int num_threads)
{
    sem_init(&b->sem, 0, 0);
    sem_init(&b->lock, 0, 1);
    b->threshold = num_threads;
    b->waiters = 0;
}

void barrier(barrier_t *b)
{
    int i;

    sleep(1);

    sem_wait(&b->lock);
    b->waiters++;
    
    if (b->waiters == b->threshold) {
        for (i = 0; i < b->threshold; ++i) {
            sem_post(&b->sem);
        }
        b->waiters = 0;
        sem_post(&b->lock);
    } else {
        sem_post(&b->lock);
        sem_wait(&b->sem);
    }
}


//
// XXX: don't change below here (just run it!)
//
typedef struct __tinfo_t {
    int thread_id;
} tinfo_t;

void *child(void *arg)
{
    tinfo_t *t = (tinfo_t *) arg;
    printf("child %d: before\n", t->thread_id);
    barrier(&b);
    printf("child %d: after\n", t->thread_id);

    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "usage: %s <numthreads>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int num_threads = atoi(argv[1]);
    if (num_threads <= 0) {
        fprintf(stderr, "number of threads must be positive\n");
        exit(EXIT_FAILURE);
    }

    // uses VLA
    pthread_t p[num_threads];
    tinfo_t t[num_threads];

    printf("parent: begin\n");
    barrier_init(&b, num_threads);

    int i;
    for (i = 0; i < num_threads; i++) {
        t[i].thread_id = i;
        if (i % 2) {
            sleep(1);
        }
        pthread_create(&p[i], NULL, child, &t[i]);
    }

    for (i = 0; i < num_threads; i++)
        pthread_join(p[i], NULL);

    printf("parent: end\n");
    return 0;
}


