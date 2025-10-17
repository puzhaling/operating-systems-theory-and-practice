/*
 * Simple concurrent counter perfomance test with multiple threads.
 * Counter must reach LIMIT value in the end. 
 */

#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>

#define LIMIT 1e6

typedef struct __counter_t {
    int value;
    pthread_mutex_t mut;
} counter_t;

void counter_init(counter_t *c)
{
    c->value = 0;
    pthread_mutex_init(&c->mut, NULL);
}

int inc(counter_t *c)
{
    int rv;

    pthread_mutex_lock(&c->mut); 
    rv = ++c->value;
    pthread_mutex_unlock(&c->mut);
    
    return rv;
}
   
int dec(counter_t *c)
{
    int rv;

    pthread_mutex_lock(&c->mut); 
    rv = --c->value;
    pthread_mutex_unlock(&c->mut);

    return rv;
}

int get(counter_t *c)
{
    int rv;

    pthread_mutex_lock(&c->mut);
    rv = c->value;
    pthread_mutex_unlock(&c->mut);

    return rv;
}

void 
printtime(struct timeval pre, struct timeval aft, counter_t c, int nthreads)
{
    suseconds_t time;
    
    time = (aft.tv_sec * 1e6 - pre.tv_sec * 1e6) + 
            aft.tv_usec - pre.tv_usec;

    printf("value=%d; usecs spent for %d threads is: %ld\n", c.value,
              nthreads, time);
}

void *incloop(void *arg)
{
    counter_t *c = (counter_t *) arg;
    while (1) {
        pthread_mutex_lock(&c->mut);
        if (c->value >= LIMIT) {
            pthread_mutex_unlock(&c->mut);
            break;
        }
        ++c->value;
        pthread_mutex_unlock(&c->mut);
    }
    return NULL;
}

void test_for_n_threads(int nthreads)
{
    counter_t counter;
    struct timeval pre, aft;
    int i;
    pthread_t threads[nthreads];

    counter_init(&counter);
    
    gettimeofday(&pre, NULL);
    for (i = 0; i < nthreads; ++i) {
        pthread_create(&threads[i], NULL, incloop, (void *) &counter);
    }
    for (i = 0; i < nthreads; ++i) {
        pthread_join(threads[i], NULL);
    }
    gettimeofday(&aft, NULL);

    printtime(pre, aft, counter, nthreads);

    pthread_mutex_destroy(&counter.mut);
}

int main(void)
{
    test_for_n_threads(1);
    test_for_n_threads(2);
    test_for_n_threads(4);
    test_for_n_threads(8);
    test_for_n_threads(16);
    test_for_n_threads(32);
    test_for_n_threads(64);
    test_for_n_threads(128);

    return 0;
}
