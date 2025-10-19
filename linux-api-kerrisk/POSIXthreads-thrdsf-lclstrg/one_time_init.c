#include <pthread.h>
#include "../essentials.h"

typedef struct __once_init_t {
        pthread_mutex_t mtx;
        Boolean flg;
} once_init_t;

static once_init_t once = 
        { 
          .mtx = PTHREAD_MUTEX_INITIALIZER,
          .flg = FALSE
        };

int one_time_init(once_init_t *once_control,
        void (*init_routine)(void)) 
{
        int rc;
        
        rc = pthread_mutex_lock(&once_control->mtx);
        if (rc != 0)
                errExitEN(rc, "pthread_mutex_lock");
        if (once_control->flg == FALSE) {
                init_routine();
                once_control->flg = TRUE;
        }
#ifdef DEBUG
        else {
        printf("one_time_init: already done earlier\n");
        }
#endif
        rc = pthread_mutex_unlock(&once_control->mtx);
        if (rc != 0)
                errExitEN(rc, "pthread_mutex_unlock");

        return 0;
}

void routine(void) {
        printf("you won't see me again\n");
}

void *thread_routine(void *)
{
        one_time_init(&once, routine);

        pthread_exit(NULL);
}

int main(void) {
        pthread_t t1, t2;
        int rc;

        rc = pthread_create(&t1, NULL, thread_routine, NULL);
        if (rc != 0)
                errExitEN(rc, "pthread_create");

        rc = pthread_create(&t2, NULL, thread_routine, NULL);
        if (rc != 0)
                errExitEN(rc, "pthread_create");
        
        rc = pthread_join(t1, NULL);
        if (rc != 0)
                errExitEN(rc, "pthread_join");
        rc = pthread_join(t2, NULL);
        if (rc != 0)
                errExitEN(rc, "pthread_join");

        exit(EXIT_SUCCESS);
}
