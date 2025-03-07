#ifndef __essentials_h__
#define __essentials_h__

#include <stdlib.h>
#include <stdio.h>

#define NO_EINTR(stmt) while ((stmt) == -1 && errno == EINTR);

void errExit(const char *msg)
{
        perror(msg);
        exit(EXIT_FAILURE);
}


#endif // __essentials_h__
