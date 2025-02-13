#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdlib.h>

#define NUMCPUS         8
#define LIMIT           1e7
#define NUM_THREADS     4

typedef struct __counter_t {
    int global;
    pthread_mutex_t glock;
    int local[NUMCPUS];
    pthread_mutex_t llock[NUMCPUS];
    int threshold;
} counter_t;

counter_t c;

void init(counter_t *c, int threshold) {
    c->threshold = threshold; 
    c->global = 0;
    pthread_mutex_init(&c->glock, NULL);
    int i;
    for (i = 0; i < NUMCPUS; ++i) {
        c->local[i] = 0;
        pthread_mutex_init(&c->llock[i], NULL); 
    }
}

void update(counter_t *c, int threadID, int amt) {
    int cpu = threadID % NUMCPUS;
    pthread_mutex_lock(&c->llock[cpu]);
    c->local[cpu] += amt;
    if (c->local[cpu] >= c->threshold) {
        pthread_mutex_lock(&c->glock); 
        c->global += c->local[cpu];
        pthread_mutex_unlock(&c->glock);
        c->local[cpu] = 0;
    }
    pthread_mutex_unlock(&c->llock[cpu]);
}

int get(counter_t *c) {
    pthread_mutex_lock(&c->glock);
    int val = c->global;
    pthread_mutex_unlock(&c->glock);
    return val;
}

void *worker(void *arg) {
    int threadID = *((int *)arg);
    for (int i = 0; i < LIMIT / NUM_THREADS; ++i) {
        update(&c, threadID, 1);
    }
    return NULL;
}

void print_time(struct timeval start, struct timeval end, counter_t *c) {
    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;
    double elapsed = seconds + microseconds * 1e-6;
    printf("Counter value: %d\n", get(c));
    printf("Time elapsed: %.6f seconds\n", elapsed);
}

int main(void) {
    init(&c, 1024);

    pthread_t threads[NUM_THREADS];
    int threadIDs[NUM_THREADS];
    struct timeval start, end;

    gettimeofday(&start, NULL);
    for (int i = 0; i < NUM_THREADS; ++i) {
        threadIDs[i] = i;
        pthread_create(&threads[i], NULL, worker, &threadIDs[i]);
    }

    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }
    gettimeofday(&end, NULL);

    print_time(start, end, &c);

    pthread_mutex_destroy(&c.glock);
    for (int i = 0; i < NUMCPUS; ++i) {
        pthread_mutex_destroy(&c.llock[i]);
    }

    return 0;
}
