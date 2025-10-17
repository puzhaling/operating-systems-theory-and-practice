#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t s;

void *child(void *)
{
    printf("child\n");
    sleep(1);

    sem_post(&s);

    return NULL;
}

int main(void)
{
    pthread_t p;
    
    // use semaphore as conditional variable here
    sem_init(&s, 0, 0);

    pthread_create(&p, NULL, child, NULL);
    sem_wait(&s);

    printf("parent: end\n");
    return 0;
}
