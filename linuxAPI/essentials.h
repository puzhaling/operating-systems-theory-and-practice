#ifndef __essentials_h__
#define __essentials_h__

#include <stdlib.h>
#include <stdio.h>

void errExit(const char *msg)
{
        perror(msg);
        exit(EXIT_FAILURE);
}

#endif // __essentials_h__
