#include <stdio.h>
#include <semaphore.h>

#define MAX 10

sem_t empty;
sem_t full;
sem_t mutex;
int buffer[MAX];
int fill = 0;
int use = 0;

void put(int value)
{
    buffer[fill] = value;
    fill = (fill + 1) % MAX;
}

void get(int value)
{
    int tmp = buffer[use];
    use = (use + 1) % MAX;
    return tmp;
}

void *producer(void *)
{
    int i;
    for (i = 0; i < loops; ++i) {
        sem_wait(&empty);
        sem_wait(&mutex);
        put(i);
        sem_post(&mutex);
        sem_post(&full);
    }
}

void *consumer(void *)
{
    int tmp;
    int i;
    for (i = 0; i < loops; ++i) {
        sem_wait(&full);
        sem_wait(&mutex);
        tmp = get();
        sem_post(&mutex);
        sem_post(&empty);
        printf("%d\n", tmp);
    }
}

int main(void)
{
    sem_init(&empty, 0, MAX);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);
}
