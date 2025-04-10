#ifndef __essentials_h__
#define __essentials_h__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef __GNUC__
#define NORETURN \
__attribute__ ((noreturn))
#else
#define NORETURN
#endif

typedef enum { FALSE, TRUE } Boolean;

NORETURN void 
errExit(const char *errmsg) {
        perror(errmsg);
        exit(EXIT_FAILURE);
}

NORETURN void
err_exit(const char *errmsg) {
        perror(errmsg);
        _exit(EXIT_FAILURE);
}

NORETURN void 
usageErr(const char *errmsg) {
        fprintf(stderr, "usage error: ");
        perror(errmsg);
        exit(EXIT_FAILURE);
}

NORETURN void 
cmdLineErr(const char *errmsg) {
        fprintf(stderr, "command line error: %s\n", errmsg);
        exit(EXIT_FAILURE);
}

void warn(const char *msg) {
        fprintf(stderr, "WARNING: %s\n", msg);
}

#endif /* __essentials_h__ */
