#include <unistd.h>
#include <signal.h>
#include <stdio.h>

void coredumpgen()
{
    switch (fork())
        {
        case -1:
            fprintf(stderr, "fork");
            break;
        case 0: /* child */
            raise(SIGABRT);
            break;
        default:
            break;
        }
}

int main(void)
{
    /* ... */
    coredumpgen();
    /* ... */
}
