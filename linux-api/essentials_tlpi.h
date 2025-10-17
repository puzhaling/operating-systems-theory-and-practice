#ifndef __essentials_h__
#define __essentials_h__

#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <errno.h>
#include <string.h>

typedef enum { FALSE, TRUE } Boolean;

#include "error_functions.h"

#define NO_EINTR(stmt) while ((stmt) == -1 && errno == EINTR);

#define min(m,n) ((m) < (n) ? (m) : (n))
#define max(m,n) ((m) > (n) ? (m) : (n))

#endif /* __essentials_h__ */
