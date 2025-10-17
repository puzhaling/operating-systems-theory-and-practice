#include <stdio.h>
#include <pthread.h>
#include <stddef.h>

int main(void)
{
    int s;
    s = pthread_join(pthread_self(), NULL);  // UB

    // how to such a situation?
    pthread_t tid = // suppose we have a thread id 
    // and we are unsure it is thread id of executing
    // process, or not.

    // to prevent UB, showed previously, we can do this:
    if (!pthread_equal(tid, pthread_self())
        // ...
}
