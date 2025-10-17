#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char **environ;

int setenv(const char *name, const char *value, int overwrite)
{
    static char ebuf[1024];
    char *ep, *destp;
    int rc = 0;
    if ( ((ep = getenv(name)) == NULL) || (ep && overwrite) ) {
        destp = stpcpy(ebuf, name);
        *(destp++) = '=';
        destp = stpcpy(destp, value);
        *destp = '\0';
        rc = putenv(ebuf);
    } 
    return rc;
}

void swap(void *lhs, void *rhs)
{
    void *temp = lhs;
    lhs = rhs;
    rhs = temp;
}

int unsetenv(const char *name)
{
    char **ep;
    char *eq;
    for (ep = environ; *ep != NULL; ++ep) {
        eq = strchr(*ep, '=');
        if (strncmp(name, *ep, eq - *ep) == 0) {
            shiftenvl(ep); 
        }
    }  
}
