/*
 * attemps to accomplish a non-local return to the
 * function that was already returned
 */

#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

static jmp_buf env;

void foo()
{
    switch (setjmp(env)) {
    case 0:
        puts("initial return");
        break;
    default:
        puts("wtf#?!@");
        break;
    }
}

int main(void)
{
#ifdef OPTIMIZED
    puts("optimized");
#else
    puts("not optimized");
#endif

    foo();   
    longjmp(env, 1);
    exit(EXIT_SUCCESS);
}
