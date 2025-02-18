#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

typedef struct __node_t {
   sem_t sem;
   struct __node_t *next;
} node_t;

typedef struct __lock_queue_t {
    sem_t glock;
    node_t *head;
    node_t *tail;
} lock_queue_t;

typedef struct __ns_mutex_t {
    sem_t glock;    
    lock_queue_t lock_queue;    
} ns_mutex_t;


void node_init(node_t *n)
{
    sem_init(&n->sem, 0, 1);
    n->next = NULL;
}

void node_free(node_t *n)
{
    sem_destroy(&n->sem);    
    free(n);
}

void lock_queue_init(lock_queue_t *q)
{
   node_t *new_node = malloc(sizeof(node_t));
   q->head = q->tail = new_node;
   new_node->next = NULL;
   sem_init(&q->glock, 0, 1);
}

void lock_queue_enqueue(lock_queue_t *q)
{
    node_t *new_node = malloc(sizeof(node_t));
    node_init(new_node);

    sem_wait(&q->glock);
    q->tail->next = new_node;
    q->tail = new_node;
    sem_post(&q->glock);

    sem_wait(&new_node->sem);
}

void lock_queue_dequeue(lock_queue_t *q)
{
    node_t *curr, *next;

    sem_wait(&q->glock);
    if (q->head->next == NULL) {
        fprintf(stderr, "queue is empty\n");
    } else {
        curr = q->head->next;
        next = curr->next;
        q->head->next = next;
        sem_post(&curr->sem);
        node_free(curr);
    }
    sem_post(&q->glock);
}

void ns_mutex_init(ns_mutex_t *m)
{
    sem_init(&m->glock, 0, 1);
    lock_queue_init(&m->lock_queue);
}


void ns_mutex_acquire(ns_mutex_t *m)
{
    lock_queue_enqueue(&m->lock_queue);
}

void ns_mutex_release(ns_mutex_t *m)
{
    lock_queue_dequeue(&m->lock_queue);
}


typedef struct __worker_arg_t {
    ns_mutex_t *m; 
    int threadID;
} worker_arg_t;

void *worker(void *arg)
{
    worker_arg_t *_arg= (worker_arg_t *)arg;
    ns_mutex_t *m = _arg->m;
    int threadID = _arg->threadID;

    ns_mutex_acquire(m);
    printf("Thread %ld acquired the mutex\n", threadID);
    sleep(1);
    printf("Thread %ld releasing the mutex\n", threadID);
    ns_mutex_release(m);

    return NULL;
}

int main(void)
{
    printf("parent: begin\n");

    ns_mutex_t mutex;
    ns_mutex_init(&mutex);
    
    pthread_t threads[10];
    for (int i = 0; i < 10; i++) {
        worker_arg_t *arg = malloc(sizeof(worker_arg_t));
        arg->m = &mutex;
        arg->threadID = i;
        pthread_create(&threads[i], NULL, worker, arg);
    }

    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("parent: end\n");
    return 0;
}
